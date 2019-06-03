/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#pragma once

#include "logdevice/common/PermissionChecker.h"
#include "logdevice/common/protocol/FINDKEY_Message.h"
#include "logdevice/common/protocol/Message.h"

namespace facebook { namespace logdevice {

struct Address;

Message::Disposition
FINDKEY_onReceived(FINDKEY_Message* msg,
                   const Address& from,
                   PermissionCheckStatus permission_status);
}} // namespace facebook::logdevice
