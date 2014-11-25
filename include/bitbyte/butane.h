//===-- bitbyte/butane.h ----------------------------------------*- C++ -*-===//
//
//  Butane
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@bitbyte.ca>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Imports Butane into the global namespace.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_H_
#define _BITBYTE_BUTANE_H_

//============================================================================//

/// \def __BITBYTE_BUTANE_IMPORT__
/// \brief Defined implictly by bitbyte/butane.h it results in the C-api
/// of Butane being "imported" into the global namespace, that is to say,
/// bitbyte_butane_xxx() becomes butane_xxx().
///
#define __BITBYTE_BUTANE_IMPORT__

//============================================================================//

#ifdef __cplusplus

//===----------------------------------------------------------------------===//

/// \namespace ::bitbyte::butane
/// \brief A data-driven game engine.
///
namespace bitbyte { namespace butane {} }
using namespace ::bitbyte::butane;

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//

#include "bitbyte/butane/config.h"
#include "bitbyte/butane/linkage.h"
#include "bitbyte/butane/foundation.h"

//===----------------------------------------------------------------------===//

#include "bitbyte/butane/application.h"
#include "bitbyte/butane/time_step_policy.h"

//============================================================================//

#endif // _BITBYTE_BUTANE_H_

//============================================================================//
