/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "logdevice/common/PrincipalIdentity.h"

#include <sstream>

#include <folly/hash/Hash.h>

//#include "logdevice/common/debug.h"

namespace facebook { namespace logdevice {

constexpr const char* PrincipalIdentity::IDENTITY_USER;
constexpr const char* PrincipalIdentity::IDENTITY_SERVICE;
constexpr const char* PrincipalIdentity::IDENTITY_TIER;
constexpr const char* PrincipalIdentity::IDENTITY_MACHINE;
constexpr const char* PrincipalIdentity::IDENTITY_JOB;

PrincipalIdentity::PrincipalIdentity(const std::string& type) : type(type) {}

PrincipalIdentity::PrincipalIdentity(
    const std::string& type,
    const std::pair<std::string, std::string>& identity)
    : type(type), primary_identity(identity) {
  identities.push_back(identity);
  calculateIdentitiesHash();
}

PrincipalIdentity::PrincipalIdentity(
    const std::string& type,
    const std::pair<std::string, std::string>& identity,
    const std::vector<std::pair<std::string, std::string>>& identities)
    : type(type), identities(identities), primary_identity(identity) {
  calculateIdentitiesHash();
}

void PrincipalIdentity::calculateIdentitiesHash() {
  folly::hash::SpookyHashV2 hasher;
  hasher.Init(0, 0);
  if (!identities.size()) {
    hasher.Update(
        (void*)primary_identity.first.c_str(), primary_identity.first.size());
    hasher.Update(
        (void*)primary_identity.second.c_str(), primary_identity.second.size());
  }
  for (auto& identity : identities) {
    hasher.Update((void*)identity.first.c_str(), identity.first.size());
    hasher.Update((void*)identity.second.c_str(), identity.second.size());
  }
  uint64_t h2;
  hasher.Final(&identities_hash, &h2);
}

std::string PrincipalIdentity::toString() const {
  std::ostringstream oss;
  oss << "Principal type: " << type << ", Identities: ";
  for (auto identity : identities) {
    oss << identity.first << ":" << identity.second << " ";
  }
  return oss.str();
}

bool PrincipalIdentity::isValidIdentityType(const std::string& idType) {
  return idType == IDENTITY_USER || idType == IDENTITY_SERVICE ||
      idType == IDENTITY_TIER || idType == IDENTITY_MACHINE ||
      idType == IDENTITY_JOB;
}

}} // namespace facebook::logdevice
