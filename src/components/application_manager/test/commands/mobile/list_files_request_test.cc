/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string>
#include <utility>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "mobile/list_files_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/smart_object_keys.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace list_files_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::DoAll;
using ::testing::SaveArg;
namespace am = ::application_manager;
using am::commands::ListFilesRequest;
using am::commands::MessageSharedPtr;

class ListFilesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    app_ = CreateMockApp();
  }
  void SetExpectCallsForRunSuccess() {
    const std::string file_name("file_name");
    files_map_.insert(
        std::pair<std::string, am::AppFile>(file_name, app_file_));

    ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));
    ON_CALL(*app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));

    ON_CALL(*app_, increment_list_files_in_none_count())
        .WillByDefault(Return());

    ON_CALL(*app_, GetAvailableDiskSpace()).WillByDefault(Return(0));
    ON_CALL(*app_, getAppFiles()).WillByDefault(ReturnRef(files_map_));
    EXPECT_CALL(mock_app_manager_, get_settings())
        .WillOnce(ReturnRef(mock_app_manager_settings_));
  }
  MockAppPtr app_;
  am::AppFile app_file_;
  am::AppFilesMap files_map_;
};

TEST_F(ListFilesRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  SharedPtr<ListFilesRequest> command(CreateCommand<ListFilesRequest>());

  ON_CALL(mock_app_manager_, application(_))
      .WillByDefault(Return(SharedPtr<am::Application>()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(ListFilesRequestTest, Run_TooManyHmiNone_UNSUCCESS) {
  SharedPtr<ListFilesRequest> command(CreateCommand<ListFilesRequest>());

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));
  ON_CALL(*app_, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_NONE));

  const uint32_t kListFilesInNoneAllowed = 1u;
  const uint32_t kListFilesInNoneCount = 2u;

  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  ON_CALL(mock_app_manager_settings_, list_files_in_none())
      .WillByDefault(ReturnRef(kListFilesInNoneAllowed));
  ON_CALL(*app_, list_files_in_none_count())
      .WillByDefault(Return(kListFilesInNoneCount));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(ListFilesRequestTest, Run_SUCCESS) {
  SharedPtr<ListFilesRequest> command(CreateCommand<ListFilesRequest>());

  SetExpectCallsForRunSuccess();

  const uint32_t files_response_cnt = 1;
  EXPECT_CALL(mock_app_manager_settings_, list_files_response_size())
      .WillOnce(ReturnRef(files_response_cnt));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::SUCCESS,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(ListFilesRequestTest, Run_EmptyListFiles_SUCCESS) {
  SharedPtr<ListFilesRequest> command(CreateCommand<ListFilesRequest>());

  SetExpectCallsForRunSuccess();

  const uint32_t files_response_cnt = 0;
  EXPECT_CALL(mock_app_manager_settings_, list_files_response_size())
      .WillOnce(ReturnRef(files_response_cnt));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::SUCCESS,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

}  // namespace list_files_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test