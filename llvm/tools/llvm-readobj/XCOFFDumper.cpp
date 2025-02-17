//===-- XCOFFDumper.cpp - XCOFF dumping utility -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements an XCOFF specific dumper for llvm-readobj.
//
//===----------------------------------------------------------------------===//

#include "Error.h"
#include "ObjDumper.h"
#include "llvm-readobj.h"
#include "llvm/Object/XCOFFObjectFile.h"
#include "llvm/Support/ScopedPrinter.h"

using namespace llvm;
using namespace object;

namespace {

class XCOFFDumper : public ObjDumper {
  enum {
    SymbolTypeMask = 0x07,
    SymbolAlignmentMask = 0xF8,
    SymbolAlignmentBitOffset = 3
  };

public:
  XCOFFDumper(const XCOFFObjectFile &Obj, ScopedPrinter &Writer)
      : ObjDumper(Writer), Obj(Obj) {}

  void printFileHeaders() override;
  void printSectionHeaders() override;
  void printRelocations() override;
  void printSymbols() override;
  void printDynamicSymbols() override;
  void printUnwindInfo() override;
  void printStackMap() const override;
  void printNeededLibraries() override;

private:
  template <typename T> void printSectionHeaders(ArrayRef<T> Sections);
  void printFileAuxEnt(const XCOFFFileAuxEnt *AuxEntPtr);
  void printCsectAuxEnt32(const XCOFFCsectAuxEnt32 *AuxEntPtr);
  void printSectAuxEntForStat(const XCOFFSectAuxEntForStat *AuxEntPtr);
  void printSymbol(const SymbolRef &);

  // Least significant 3 bits are reserved.
  static constexpr unsigned SectionFlagsReservedMask = 0x7;
  const XCOFFObjectFile &Obj;
};
} // anonymous namespace

void XCOFFDumper::printFileHeaders() {
  DictScope DS(W, "FileHeader");
  W.printHex("Magic", Obj.getMagic());
  W.printNumber("NumberOfSections", Obj.getNumberOfSections());

  // Negative timestamp values are reserved for future use.
  int32_t TimeStamp = Obj.getTimeStamp();
  if (TimeStamp > 0) {
    // This handling of the time stamp assumes that the host system's time_t is
    // compatible with AIX time_t. If a platform is not compatible, the lit
    // tests will let us know.
    time_t TimeDate = TimeStamp;

    char FormattedTime[21] = {};
    size_t BytesWritten =
        strftime(FormattedTime, 21, "%Y-%m-%dT%H:%M:%SZ", gmtime(&TimeDate));
    if (BytesWritten)
      W.printHex("TimeStamp", FormattedTime, TimeStamp);
    else
      W.printHex("Timestamp", TimeStamp);
  } else {
    W.printHex("TimeStamp", TimeStamp == 0 ? "None" : "Reserved Value",
               TimeStamp);
  }

  // The number of symbol table entries is an unsigned value in 64-bit objects
  // and a signed value (with negative values being 'reserved') in 32-bit
  // objects.
  if (Obj.is64Bit()) {
    W.printHex("SymbolTableOffset", Obj.getSymbolTableOffset64());
    W.printNumber("SymbolTableEntries", Obj.getNumberOfSymbolTableEntries64());
  } else {
    W.printHex("SymbolTableOffset", Obj.getSymbolTableOffset32());
    int32_t SymTabEntries = Obj.getRawNumberOfSymbolTableEntries32();
    if (SymTabEntries >= 0)
      W.printNumber("SymbolTableEntries", SymTabEntries);
    else
      W.printHex("SymbolTableEntries", "Reserved Value", SymTabEntries);
  }

  W.printHex("OptionalHeaderSize", Obj.getOptionalHeaderSize());
  W.printHex("Flags", Obj.getFlags());

  // TODO FIXME Add support for the auxiliary header (if any) once
  // XCOFFObjectFile has the necessary support.
}

void XCOFFDumper::printSectionHeaders() {
  if (Obj.is64Bit())
    printSectionHeaders(Obj.sections64());
  else
    printSectionHeaders(Obj.sections32());
}

void XCOFFDumper::printRelocations() {
  llvm_unreachable("Unimplemented functionality for XCOFFDumper");
}

static const EnumEntry<XCOFF::CFileStringType> FileStringType[] = {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
    ECase(XFT_FN), ECase(XFT_CT), ECase(XFT_CV), ECase(XFT_CD)
#undef ECase
};

void XCOFFDumper::printFileAuxEnt(const XCOFFFileAuxEnt *AuxEntPtr) {
  if (Obj.is64Bit())
    report_fatal_error(
        "Printing for File Auxiliary Entry in 64-bit is unimplemented.");
  StringRef FileName =
      unwrapOrError(Obj.getFileName(), Obj.getCFileName(AuxEntPtr));
  DictScope SymDs(W, "File Auxiliary Entry");
  W.printNumber("Index",
                Obj.getSymbolIndex(reinterpret_cast<uintptr_t>(AuxEntPtr)));
  W.printString("Name", FileName);
  W.printEnum("Type", static_cast<uint8_t>(AuxEntPtr->Type),
              makeArrayRef(FileStringType));
}

static const EnumEntry<XCOFF::StorageMappingClass> CsectStorageMappingClass[] =
    {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
        ECase(XMC_PR),   ECase(XMC_RO),     ECase(XMC_DB),
        ECase(XMC_GL),   ECase(XMC_XO),     ECase(XMC_SV),
        ECase(XMC_SV64), ECase(XMC_SV3264), ECase(XMC_TI),
        ECase(XMC_TB),   ECase(XMC_RW),     ECase(XMC_TC0),
        ECase(XMC_TC),   ECase(XMC_TD),     ECase(XMC_DS),
        ECase(XMC_UA),   ECase(XMC_BS),     ECase(XMC_UC),
        ECase(XMC_TL),   ECase(XMC_TE)
#undef ECase
};

static const EnumEntry<XCOFF::SymbolType> CsectSymbolTypeClass[] = {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
    ECase(XTY_ER), ECase(XTY_SD), ECase(XTY_LD), ECase(XTY_CM)
#undef ECase
};

void XCOFFDumper::printCsectAuxEnt32(const XCOFFCsectAuxEnt32 *AuxEntPtr) {
  assert(!Obj.is64Bit() && "32-bit interface called on 64-bit object file.");

  DictScope SymDs(W, "CSECT Auxiliary Entry");
  W.printNumber("Index",
                Obj.getSymbolIndex(reinterpret_cast<uintptr_t>(AuxEntPtr)));
  if ((AuxEntPtr->SymbolAlignmentAndType & SymbolTypeMask) == XCOFF::XTY_LD)
    W.printNumber("ContainingCsectSymbolIndex", AuxEntPtr->SectionLen);
  else
    W.printNumber("SectionLen", AuxEntPtr->SectionLen);
  W.printHex("ParameterHashIndex", AuxEntPtr->ParameterHashIndex);
  W.printHex("TypeChkSectNum", AuxEntPtr->TypeChkSectNum);
  // Print out symbol alignment and type.
  W.printNumber("SymbolAlignmentLog2",
                (AuxEntPtr->SymbolAlignmentAndType & SymbolAlignmentMask) >>
                    SymbolAlignmentBitOffset);
  W.printEnum("SymbolType", AuxEntPtr->SymbolAlignmentAndType & SymbolTypeMask,
              makeArrayRef(CsectSymbolTypeClass));
  W.printEnum("StorageMappingClass",
              static_cast<uint8_t>(AuxEntPtr->StorageMappingClass),
              makeArrayRef(CsectStorageMappingClass));
  W.printHex("StabInfoIndex", AuxEntPtr->StabInfoIndex);
  W.printHex("StabSectNum", AuxEntPtr->StabSectNum);
}

void XCOFFDumper::printSectAuxEntForStat(
    const XCOFFSectAuxEntForStat *AuxEntPtr) {
  assert(!Obj.is64Bit() && "32-bit interface called on 64-bit object file.");

  DictScope SymDs(W, "Sect Auxiliary Entry For Stat");
  W.printNumber("Index",
                Obj.getSymbolIndex(reinterpret_cast<uintptr_t>(AuxEntPtr)));
  W.printNumber("SectionLength", AuxEntPtr->SectionLength);

  // Unlike the corresponding fields in the section header, NumberOfRelocEnt
  // and NumberOfLineNum do not handle values greater than 65535.
  W.printNumber("NumberOfRelocEnt", AuxEntPtr->NumberOfRelocEnt);
  W.printNumber("NumberOfLineNum", AuxEntPtr->NumberOfLineNum);
}

static const EnumEntry<XCOFF::StorageClass> SymStorageClass[] = {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
    ECase(C_NULL),  ECase(C_AUTO),    ECase(C_EXT),     ECase(C_STAT),
    ECase(C_REG),   ECase(C_EXTDEF),  ECase(C_LABEL),   ECase(C_ULABEL),
    ECase(C_MOS),   ECase(C_ARG),     ECase(C_STRTAG),  ECase(C_MOU),
    ECase(C_UNTAG), ECase(C_TPDEF),   ECase(C_USTATIC), ECase(C_ENTAG),
    ECase(C_MOE),   ECase(C_REGPARM), ECase(C_FIELD),   ECase(C_BLOCK),
    ECase(C_FCN),   ECase(C_EOS),     ECase(C_FILE),    ECase(C_LINE),
    ECase(C_ALIAS), ECase(C_HIDDEN),  ECase(C_HIDEXT),  ECase(C_BINCL),
    ECase(C_EINCL), ECase(C_INFO),    ECase(C_WEAKEXT), ECase(C_DWARF),
    ECase(C_GSYM),  ECase(C_LSYM),    ECase(C_PSYM),    ECase(C_RSYM),
    ECase(C_RPSYM), ECase(C_STSYM),   ECase(C_TCSYM),   ECase(C_BCOMM),
    ECase(C_ECOML), ECase(C_ECOMM),   ECase(C_DECL),    ECase(C_ENTRY),
    ECase(C_FUN),   ECase(C_BSTAT),   ECase(C_ESTAT),   ECase(C_GTLS),
    ECase(C_STTLS), ECase(C_EFCN)
#undef ECase
};

static StringRef GetSymbolValueName(XCOFF::StorageClass SC) {
  switch (SC) {
  case XCOFF::C_EXT:
  case XCOFF::C_WEAKEXT:
  case XCOFF::C_HIDEXT:
  case XCOFF::C_STAT:
    return "Value (RelocatableAddress)";
  case XCOFF::C_FILE:
    return "Value (SymbolTableIndex)";
  case XCOFF::C_FCN:
  case XCOFF::C_BLOCK:
  case XCOFF::C_FUN:
  case XCOFF::C_STSYM:
  case XCOFF::C_BINCL:
  case XCOFF::C_EINCL:
  case XCOFF::C_INFO:
  case XCOFF::C_BSTAT:
  case XCOFF::C_LSYM:
  case XCOFF::C_PSYM:
  case XCOFF::C_RPSYM:
  case XCOFF::C_RSYM:
  case XCOFF::C_ECOML:
  case XCOFF::C_DWARF:
    assert(false && "This StorageClass for the symbol is not yet implemented.");
  default:
    return "Value";
  }
}

static const EnumEntry<XCOFF::CFileLangId> CFileLangIdClass[] = {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
    ECase(TB_C), ECase(TB_CPLUSPLUS)
#undef ECase
};

static const EnumEntry<XCOFF::CFileCpuId> CFileCpuIdClass[] = {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
    ECase(TCPU_PPC64), ECase(TCPU_COM), ECase(TCPU_970)
#undef ECase
};

void XCOFFDumper::printSymbol(const SymbolRef &S) {
  if (Obj.is64Bit())
    report_fatal_error("64-bit support is unimplemented.");

  DataRefImpl SymbolDRI = S.getRawDataRefImpl();
  const XCOFFSymbolEntry *SymbolEntPtr = Obj.toSymbolEntry(SymbolDRI);

  XCOFFSymbolRef XCOFFSymRef(SymbolDRI, &Obj);
  uint8_t NumberOfAuxEntries = XCOFFSymRef.getNumberOfAuxEntries();

  DictScope SymDs(W, "Symbol");

  StringRef SymbolName =
      unwrapOrError(Obj.getFileName(), Obj.getSymbolName(SymbolDRI));

  W.printNumber("Index",
                Obj.getSymbolIndex(reinterpret_cast<uintptr_t>(SymbolEntPtr)));
  W.printString("Name", SymbolName);
  W.printHex(GetSymbolValueName(SymbolEntPtr->StorageClass),
             SymbolEntPtr->Value);

  StringRef SectionName =
      unwrapOrError(Obj.getFileName(), Obj.getSymbolSectionName(SymbolEntPtr));

  W.printString("Section", SectionName);
  if (XCOFFSymRef.getStorageClass() == XCOFF::C_FILE) {
    W.printEnum("Source Language ID",
                SymbolEntPtr->CFileLanguageIdAndTypeId.LanguageId,
                makeArrayRef(CFileLangIdClass));
    W.printEnum("CPU Version ID",
                SymbolEntPtr->CFileLanguageIdAndTypeId.CpuTypeId,
                makeArrayRef(CFileCpuIdClass));
  } else
    W.printHex("Type", SymbolEntPtr->SymbolType);

  W.printEnum("StorageClass", static_cast<uint8_t>(SymbolEntPtr->StorageClass),
              makeArrayRef(SymStorageClass));
  W.printNumber("NumberOfAuxEntries", SymbolEntPtr->NumberOfAuxEntries);

  if (NumberOfAuxEntries == 0)
    return;

  switch (XCOFFSymRef.getStorageClass()) {
  case XCOFF::C_FILE:
    // If the symbol is C_FILE and has auxiliary entries...
    for (int i = 1; i <= NumberOfAuxEntries; i++) {
      const XCOFFFileAuxEnt *FileAuxEntPtr =
          reinterpret_cast<const XCOFFFileAuxEnt *>(SymbolEntPtr + i);
#ifndef NDEBUG
      Obj.checkSymbolEntryPointer(reinterpret_cast<uintptr_t>(FileAuxEntPtr));
#endif
      printFileAuxEnt(FileAuxEntPtr);
    }
    break;
  case XCOFF::C_EXT:
  case XCOFF::C_WEAKEXT:
  case XCOFF::C_HIDEXT:
    // If the symbol is for a function, and it has more than 1 auxiliary entry,
    // then one of them must be function auxiliary entry which we do not
    // support yet.
    if (XCOFFSymRef.isFunction() && NumberOfAuxEntries >= 2)
      report_fatal_error("Function auxiliary entry printing is unimplemented.");

    // If there is more than 1 auxiliary entry, instead of printing out
    // error information, print out the raw Auxiliary entry from 1st till
    // the last - 1. The last one must be a CSECT Auxiliary Entry.
    for (int i = 1; i < NumberOfAuxEntries; i++) {
      W.startLine() << "!Unexpected raw auxiliary entry data:\n";
      W.startLine() << format_bytes(
          ArrayRef<uint8_t>(reinterpret_cast<const uint8_t *>(SymbolEntPtr + i),
                            XCOFF::SymbolTableEntrySize));
    }

    // The symbol's last auxiliary entry is a CSECT Auxiliary Entry.
    printCsectAuxEnt32(XCOFFSymRef.getXCOFFCsectAuxEnt32());
    break;
  case XCOFF::C_STAT:
    if (NumberOfAuxEntries > 1)
      report_fatal_error(
          "C_STAT symbol should not have more than 1 auxiliary entry.");

    const XCOFFSectAuxEntForStat *StatAuxEntPtr;
    StatAuxEntPtr =
        reinterpret_cast<const XCOFFSectAuxEntForStat *>(SymbolEntPtr + 1);
#ifndef NDEBUG
    Obj.checkSymbolEntryPointer(reinterpret_cast<uintptr_t>(StatAuxEntPtr));
#endif
    printSectAuxEntForStat(StatAuxEntPtr);
    break;
  case XCOFF::C_DWARF:
  case XCOFF::C_BLOCK:
  case XCOFF::C_FCN:
    report_fatal_error("Symbol table entry printing for this storage class "
                       "type is unimplemented.");
    break;
  default:
    for (int i = 1; i <= NumberOfAuxEntries; i++) {
      W.startLine() << "!Unexpected raw auxiliary entry data:\n";
      W.startLine() << format_bytes(
          ArrayRef<uint8_t>(reinterpret_cast<const uint8_t *>(SymbolEntPtr + i),
                            XCOFF::SymbolTableEntrySize));
    }
    break;
  }
}

void XCOFFDumper::printSymbols() {
  ListScope Group(W, "Symbols");
  for (const SymbolRef &S : Obj.symbols())
    printSymbol(S);
}

void XCOFFDumper::printDynamicSymbols() {
  llvm_unreachable("Unimplemented functionality for XCOFFDumper");
}

void XCOFFDumper::printUnwindInfo() {
  llvm_unreachable("Unimplemented functionality for XCOFFDumper");
}

void XCOFFDumper::printStackMap() const {
  llvm_unreachable("Unimplemented functionality for XCOFFDumper");
}

void XCOFFDumper::printNeededLibraries() {
  llvm_unreachable("Unimplemented functionality for XCOFFDumper");
}

static const EnumEntry<XCOFF::SectionTypeFlags> SectionTypeFlagsNames[] = {
#define ECase(X)                                                               \
  { #X, XCOFF::X }
    ECase(STYP_PAD),    ECase(STYP_DWARF), ECase(STYP_TEXT),
    ECase(STYP_DATA),   ECase(STYP_BSS),   ECase(STYP_EXCEPT),
    ECase(STYP_INFO),   ECase(STYP_TDATA), ECase(STYP_TBSS),
    ECase(STYP_LOADER), ECase(STYP_DEBUG), ECase(STYP_TYPCHK),
    ECase(STYP_OVRFLO)
#undef ECase
};

template <typename T>
void XCOFFDumper::printSectionHeaders(ArrayRef<T> Sections) {
  ListScope Group(W, "Sections");

  uint16_t Index = 1;
  for (const T &Sec : Sections) {
    DictScope SecDS(W, "Section");

    W.printNumber("Index", Index++);
    W.printString("Name", Sec.getName());

    W.printHex("PhysicalAddress", Sec.PhysicalAddress);
    W.printHex("VirtualAddress", Sec.VirtualAddress);
    W.printHex("Size", Sec.SectionSize);
    W.printHex("RawDataOffset", Sec.FileOffsetToRawData);
    W.printHex("RelocationPointer", Sec.FileOffsetToRelocationInfo);
    W.printHex("LineNumberPointer", Sec.FileOffsetToLineNumberInfo);

    // TODO Need to add overflow handling when NumberOfX == _OVERFLOW_MARKER
    // in 32-bit object files.
    W.printNumber("NumberOfRelocations", Sec.NumberOfRelocations);
    W.printNumber("NumberOfLineNumbers", Sec.NumberOfLineNumbers);

    // The most significant 16-bits represent the DWARF section subtype. For
    // now we just dump the section type flags.
    uint16_t Flags = Sec.Flags & 0xffffu;
    if (Flags & SectionFlagsReservedMask)
      W.printHex("Flags", "Reserved", Flags);
    else
      W.printEnum("Type", Flags, makeArrayRef(SectionTypeFlagsNames));
  }

  if (opts::SectionRelocations)
    report_fatal_error("Dumping section relocations is unimplemented");

  if (opts::SectionSymbols)
    report_fatal_error("Dumping symbols is unimplemented");

  if (opts::SectionData)
    report_fatal_error("Dumping section data is unimplemented");
}

namespace llvm {
std::error_code createXCOFFDumper(const object::ObjectFile *Obj,
                                  ScopedPrinter &Writer,
                                  std::unique_ptr<ObjDumper> &Result) {
  const XCOFFObjectFile *XObj = dyn_cast<XCOFFObjectFile>(Obj);
  if (!XObj)
    return readobj_error::unsupported_obj_file_format;

  Result.reset(new XCOFFDumper(*XObj, Writer));
  return readobj_error::success;
}
} // namespace llvm
