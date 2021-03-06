/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include <AzCore/std/string/regex.h>
#include <AzFramework/StringFunc/StringFunc.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>
#include <SceneAPI/SceneCore/Utilities/PatternMatcher.h>

namespace AZ
{
    namespace SceneAPI
    {
        namespace SceneCore
        {
            PatternMatcher::PatternMatcher(const char* pattern, MatchApproach matcher)
                : m_pattern(pattern)
                , m_matcher(matcher)
            {
            }

            PatternMatcher::PatternMatcher(const AZStd::string& pattern, MatchApproach matcher)
                : m_pattern(pattern)
                , m_matcher(matcher)
            {
            }

            PatternMatcher::PatternMatcher(AZStd::string&& pattern, MatchApproach matcher)
                : m_pattern(AZStd::move(pattern))
                , m_matcher(matcher)
            {
            }

            PatternMatcher::PatternMatcher(PatternMatcher&& rhs)
                : m_pattern(AZStd::move(rhs.m_pattern))
                , m_matcher(rhs.m_matcher)
            {
            }

            PatternMatcher& PatternMatcher::operator=(PatternMatcher&& rhs)
            {
                m_pattern = AZStd::move(rhs.m_pattern);
                m_matcher = rhs.m_matcher;
                return *this;
            }

            bool PatternMatcher::LoadFromJson(rapidjson::Document::ConstMemberIterator member)
            {
                if (!member->value.HasMember("PatternMatcher"))
                {
                    AZ_TracePrintf(Utilities::WarningWindow, "Missing element 'PatternMatcher'.");
                    return false;
                }

                if (!member->value.HasMember("Pattern"))
                {
                    AZ_TracePrintf(Utilities::WarningWindow, "Missing element 'Pattern'.");
                    return false;
                }

                const auto& patternMatcherValue = member->value["PatternMatcher"];
                if (!patternMatcherValue.IsString())
                {
                    AZ_TracePrintf(Utilities::WarningWindow, "Element 'PatternMatcher' is not a string.");
                    return false;
                }

                const auto& patternValue = member->value["Pattern"];
                if (!patternValue.IsString())
                {
                    AZ_TracePrintf(Utilities::WarningWindow, "Element 'Pattern' is not a string.");
                    return false;
                }

                if (AzFramework::StringFunc::Equal(patternMatcherValue.GetString(), "postfix"))
                {
                    m_matcher = MatchApproach::PostFix;
                }
                else if (AzFramework::StringFunc::Equal(patternMatcherValue.GetString(), "prefix"))
                {
                    m_matcher = MatchApproach::PreFix;
                }
                else if (AzFramework::StringFunc::Equal(patternMatcherValue.GetString(), "regex"))
                {
                    m_matcher = MatchApproach::Regex;
                }
                else
                {
                    AZ_TracePrintf(Utilities::WarningWindow,
                        "Element 'PatternMatcher' is no one of the available options postfix, prefix or regex.");
                    return false;
                }

                m_pattern = patternValue.GetString();

                return true;
            }

            bool PatternMatcher::MatchesPattern(const char* name, size_t nameLength) const
            {
                switch (m_matcher)
                {
                case MatchApproach::PreFix:
                    return AzFramework::StringFunc::Equal(name, m_pattern.c_str(), false, m_pattern.size());
                case MatchApproach::PostFix:
                {
                    if (m_pattern.size() > nameLength)
                    {
                        return false;
                    }
                    size_t offset = nameLength - m_pattern.size();
                    return AzFramework::StringFunc::Equal(name + offset, m_pattern.c_str());
                }
                case MatchApproach::Regex:
                {
                    AZStd::regex comparer(m_pattern, AZStd::regex::extended);
                    AZStd::smatch match;
                    return AZStd::regex_match(name, match, comparer);
                }
                default:
                    AZ_Assert(false, "Unknown option '%i' for pattern matcher.", m_matcher);
                    return false;
                }
            }

            bool PatternMatcher::MatchesPattern(const AZStd::string& name) const
            {
                return MatchesPattern(name.c_str(), name.length());
            }

            const AZStd::string& PatternMatcher::GetPattern() const
            {
                return m_pattern;
            }

            PatternMatcher::MatchApproach PatternMatcher::GetMatchApproach() const
            {
                return m_matcher;
            }
        } // SceneCore
    } // SceneAPI
} // AZ
