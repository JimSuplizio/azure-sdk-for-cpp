// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "azure/storage/common/internal/shared_key_policy.hpp"

#include "azure/storage/common/crypt.hpp"

#include <azure/core/http/http.hpp>
#include <azure/core/internal/strings.hpp>

#include <algorithm>

namespace {
/*
 * We need to imitate .Net culture-aware sorting, which is used in storage service.
 * Below tables contain sort-keys for en-US culture.
 */
const static int table_lv0[128] = {
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x71c, 0x0,   0x71f, 0x721, 0x723, 0x725,
    0x0,   0x0,   0x0,   0x72d, 0x803, 0x0,   0x0,   0x733, 0x0,   0xd03, 0xd1a, 0xd1c, 0xd1e,
    0xd20, 0xd22, 0xd24, 0xd26, 0xd28, 0xd2a, 0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,
    0xe02, 0xe09, 0xe0a, 0xe1a, 0xe21, 0xe23, 0xe25, 0xe2c, 0xe32, 0xe35, 0xe36, 0xe48, 0xe51,
    0xe70, 0xe7c, 0xe7e, 0xe89, 0xe8a, 0xe91, 0xe99, 0xe9f, 0xea2, 0xea4, 0xea6, 0xea7, 0xea9,
    0x0,   0x0,   0x0,   0x743, 0x744, 0x748, 0xe02, 0xe09, 0xe0a, 0xe1a, 0xe21, 0xe23, 0xe25,
    0xe2c, 0xe32, 0xe35, 0xe36, 0xe48, 0xe51, 0xe70, 0xe7c, 0xe7e, 0xe89, 0xe8a, 0xe91, 0xe99,
    0xe9f, 0xea2, 0xea4, 0xea6, 0xea7, 0xea9, 0x0,   0x74c, 0x0,   0x750, 0x0,
};
const static int table_lv2[128] = {
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
    0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
};
const static int table_lv4[128] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8012, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8212, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    0x0, 0x0,
};

bool comparator(const std::string& lhs, const std::string& rhs)
{
  const static std::array<const int*, 3> tables{table_lv0, table_lv2, table_lv4};
  size_t curr_level = 0;
  size_t i = 0;
  size_t j = 0;
  while (curr_level < tables.size())
  {
    if (curr_level == tables.size() - 1 && i != j)
    {
      return i > j;
    }
    const int weight1 = i < lhs.size() ? tables[curr_level][static_cast<uint8_t>(lhs[i])] : 0x1;
    const int weight2 = j < rhs.size() ? tables[curr_level][static_cast<uint8_t>(rhs[j])] : 0x1;
    if (weight1 == 0x1 && weight2 == 0x1)
    {
      i = 0;
      j = 0;
      ++curr_level;
    }
    else if (weight1 == weight2)
    {
      ++i;
      ++j;
    }
    else if (weight1 == 0)
    {
      ++i;
    }
    else if (weight2 == 0)
    {
      ++j;
    }
    else
    {
      return weight1 < weight2;
    }
  }
  return false;
}
} // namespace

namespace Azure { namespace Storage { namespace _internal {

  std::string SharedKeyPolicy::GetSignature(const Core::Http::Request& request) const
  {
    std::string string_to_sign;
    string_to_sign += request.GetMethod().ToString() + "\n";

    const auto& headers = request.GetHeaders();
    for (std::string headerName :
         {"Content-Encoding",
          "Content-Language",
          "Content-Length",
          "Content-MD5",
          "Content-Type",
          "Date",
          "If-Modified-Since",
          "If-Match",
          "If-None-Match",
          "If-Unmodified-Since",
          "Range"})
    {
      auto ite = headers.find(headerName);
      if (ite != headers.end())
      {
        if (headerName == "Content-Length" && ite->second == "0")
        {
          // do nothing
        }
        else
        {
          string_to_sign += ite->second;
        }
      }
      string_to_sign += "\n";
    }

    // canonicalized headers
    const std::string prefix = "x-ms-";
    std::vector<std::pair<std::string, std::string>> ordered_kv;
    for (auto ite = headers.lower_bound(prefix);
         ite != headers.end() && ite->first.substr(0, prefix.length()) == prefix;
         ++ite)
    {
      std::string key = Azure::Core::_internal::StringExtensions::ToLower(ite->first);
      ordered_kv.emplace_back(std::make_pair(std::move(key), ite->second));
    }
    std::sort(ordered_kv.begin(), ordered_kv.end(), [](const auto& lhs, const auto& rhs) {
      return comparator(lhs.first, rhs.first);
    });
    for (const auto& p : ordered_kv)
    {
      string_to_sign += p.first + ":" + p.second + "\n";
    }
    ordered_kv.clear();

    // canonicalized resource
    string_to_sign += "/" + m_credential->AccountName + "/" + request.GetUrl().GetPath() + "\n";
    for (const auto& query : request.GetUrl().GetQueryParameters())
    {
      std::string key = Azure::Core::_internal::StringExtensions::ToLower(query.first);
      ordered_kv.emplace_back(
          std::make_pair(Azure::Core::Url::Decode(key), Azure::Core::Url::Decode(query.second)));
    }
    std::sort(ordered_kv.begin(), ordered_kv.end());
    for (const auto& p : ordered_kv)
    {
      string_to_sign += p.first + ":" + p.second + "\n";
    }

    // remove last linebreak
    string_to_sign.pop_back();

    return Azure::Core::Convert::Base64Encode(_internal::HmacSha256(
        std::vector<uint8_t>(string_to_sign.begin(), string_to_sign.end()),
        Azure::Core::Convert::Base64Decode(m_credential->GetAccountKey())));
  }
}}} // namespace Azure::Storage::_internal
