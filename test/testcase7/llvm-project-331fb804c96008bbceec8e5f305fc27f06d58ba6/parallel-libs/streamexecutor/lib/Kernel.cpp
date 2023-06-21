//===-- Kernel.cpp - General kernel implementation ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the implementation details for kernel types.
///
//===----------------------------------------------------------------------===//

#include "streamexecutor/Kernel.h"
#include "streamexecutor/Device.h"
#include "streamexecutor/PlatformInterfaces.h"

#include "llvm/DebugInfo/Symbolize/Symbolize.h"

namespace streamexecutor {

KernelBase::KernelBase(Device *Dev, const std::string &Name,
                       const std::string &DemangledName,
                       std::unique_ptr<KernelInterface> Implementation)
    : TheDevice(Dev), Name(Name), DemangledName(DemangledName),
      Implementation(std::move(Implementation)) {}

KernelBase::~KernelBase() = default;

Expected<KernelBase> KernelBase::create(Device *Dev,
                                        const MultiKernelLoaderSpec &Spec) {
  auto MaybeImplementation = Dev->getKernelImplementation(Spec);
  if (!MaybeImplementation) {
    return MaybeImplementation.takeError();
  }
  std::string Name = Spec.getKernelName();
  std::string DemangledName =
      llvm::symbolize::LLVMSymbolizer::DemangleName(Name, nullptr);
  KernelBase Instance(Dev, Name, DemangledName,
                      std::move(*MaybeImplementation));
  return std::move(Instance);
}

} // namespace streamexecutor