// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.
// Code generated by Microsoft (R) TypeSpec Code Generator.
// Changes may cause incorrect behavior and will be lost if the code is regenerated.

#pragma once

#include "configuration_client_models.hpp"
#include "dll_import_export.hpp"

#include <azure/core/internal/client_options.hpp>
#include <azure/core/internal/extendable_enumeration.hpp>
#include <azure/core/nullable.hpp>
#include <azure/core/url.hpp>
#include <azure/core/uuid.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Azure { namespace Data { namespace AppConfiguration {
  class KeyValueFields;
  struct KeyValue;
  class SnapshotFields;
  class SnapshotStatus;
  class LabelFields;

  struct ConfigurationClientOptions final : public Azure::Core::_internal::ClientOptions
  {
    std::string ApiVersion = "2023-11-01";
  };

  struct GetKeysOptions final
  {
    std::string NextPageToken;
    std::string Name;
    std::string After;
    std::string SyncToken;
    std::string AcceptDatetime;
  };

  struct CheckKeysOptions final
  {
    std::string Name;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetKeyValuesOptions final
  {
    std::string NextPageToken;
    std::string Key;
    std::string Label;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    std::vector<KeyValueFields> Select;
    std::string Snapshot;
    std::string IfMatch;
    std::string IfNoneMatch;
    std::vector<std::string> Tags;
  };

  struct CheckKeyValuesOptions final
  {
    std::string Key;
    std::string Label;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    std::vector<KeyValueFields> Select;
    std::string Snapshot;
    std::string IfMatch;
    std::string IfNoneMatch;
    std::vector<std::string> Tags;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetKeyValueOptions final
  {
    std::string Label;
    std::vector<KeyValueFields> Select;
    std::string SyncToken;
    std::string AcceptDatetime;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct PutKeyValueOptions final
  {
    std::string Label;
    std::string SyncToken;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
    // KeyValue Entity; TODO: Implement KeyValue entity
  };

  struct DeleteKeyValueOptions final
  {
    std::string Label;
    std::string SyncToken;
    std::string IfMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct CheckKeyValueOptions final
  {
    std::string Label;
    std::string SyncToken;
    std::string AcceptDatetime;
    std::string IfMatch;
    std::string IfNoneMatch;
    std::vector<KeyValueFields> Select;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetSnapshotsOptions final
  {
    std::string NextPageToken;
    std::string Name;
    std::string After;
    std::vector<SnapshotFields> Select;
    std::vector<SnapshotStatus> Status;
    std::string SyncToken;
  };

  struct CheckSnapshotsOptions final
  {
    std::string SyncToken;
    std::string After;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetSnapshotOptions final
  {
    std::vector<SnapshotFields> Select;
    std::string SyncToken;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetOperationDetailsOptions final
  {
    Core::Uuid XMsClientRequestId{};
  };

  struct CreateSnapshotOptions final
  {
    std::string SyncToken;
  };

  struct UpdateSnapshotOptions final
  {
    std::string SyncToken;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct CheckSnapshotOptions final
  {
    std::string SyncToken;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetLabelsOptions final
  {
    std::string NextPageToken;
    std::string Name;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    std::vector<LabelFields> Select;
    Core::Uuid XMsClientRequestId{};
  };

  struct CheckLabelsOptions final
  {
    std::string Name;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    std::vector<LabelFields> Select;
    Core::Uuid XMsClientRequestId{};
  };

  struct PutLockOptions final
  {
    std::string Label;
    std::string SyncToken;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct DeleteLockOptions final
  {
    std::string Label;
    std::string SyncToken;
    std::string IfMatch;
    std::string IfNoneMatch;
    Core::Uuid XMsClientRequestId{};
  };

  struct GetRevisionsOptions final
  {
    std::string NextPageToken;
    std::string Key;
    std::string Label;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    std::vector<KeyValueFields> Select;
    std::vector<std::string> Tags;
    Core::Uuid XMsClientRequestId{};
  };

  struct CheckRevisionsOptions final
  {
    std::string Key;
    std::string Label;
    std::string SyncToken;
    std::string After;
    std::string AcceptDatetime;
    std::vector<KeyValueFields> Select;
    std::vector<std::string> Tags;
    Core::Uuid XMsClientRequestId{};
  };
}}} // namespace Azure::Data::AppConfiguration
