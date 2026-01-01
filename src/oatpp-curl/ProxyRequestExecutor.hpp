/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_curl_ProxyRequestExecutor_hpp
#define oatpp_curl_ProxyRequestExecutor_hpp

#include "./io/BodyDecoder.hpp"

#include "oatpp/web/client/RequestExecutor.hpp"

namespace oatpp { namespace curl {

/**
 * Curl request executor. Uses Curl to perform client to server requests. <br>
 * Extends &id:oatpp::web::client::RequestExecutor;.
 */
class ProxyRequestExecutor : public oatpp::web::client::RequestExecutor {
public:

  /**
   * Connection reuse mechanism for curl ProxyRequestExecutor is **NOT IMPLEMENTED** yet.<br>
   * Use dummy stub to mock the real connection and prevent the crash.
   */
  class StubConnectionHandle : public ConnectionHandle {
  };

private:
  oatpp::String m_baseUrl;
  oatpp::String m_proxyUrl;
  std::shared_ptr<io::BodyDecoder> m_bodyDecoder;
  bool m_verbose;
public:

  /**
   * Constructor.
   * @param baseUrl - base url for requests. Ex.: "https://oatpp.io/"
   * @param proxyUrl - proxy url for requests. Ex.: "http://127.0.0.1:8080"
   * @param verbose - `true` to print curl logs to `std::cout`.
   */
  ProxyRequestExecutor(const oatpp::String &baseUrl, const oatpp::String &proxyUrl, bool verbose = false);

  /**
   * Create shared ProxyRequestExecutor.
   * @param baseUrl - base url for requests. Ex.: "https://oatpp.io/"
   * @param proxyUrl - proxy url for requests. Ex.: "http://127.0.0.1:8080"
   * @param verbose - `true` to print curl logs to `std::cout`.
   * @return - `std::shared_ptr` to ProxyRequestExecutor.
   */
  static std::shared_ptr<ProxyRequestExecutor> createShared(const oatpp::String &baseUrl, const oatpp::String &proxyUrl, bool verbose = false) {
    return std::make_shared<ProxyRequestExecutor>(baseUrl, proxyUrl, verbose);
  }

  /**
   * Connection reuse mechanism for curl ProxyRequestExecutor is **NOT IMPLEMENTED** yet.<br>
   * @return - &l:ProxyRequestExecutor::StubConnectionHandle;.
   */
  std::shared_ptr<ConnectionHandle> getConnection() override;

  /**
   * Connection reuse mechanism for curl ProxyRequestExecutor is **NOT IMPLEMENTED** yet.<br>
   * @return - &l:ProxyRequestExecutor::StubConnectionHandle;.
   */
  oatpp::async::CoroutineStarterForResult<const std::shared_ptr<ConnectionHandle> &> getConnectionAsync() override;

  /**
   * Invalidate connection.
   * @param connectionHandle
   */
  void invalidateConnection(const std::shared_ptr<ConnectionHandle> &connectionHandle) override {
    // DO NOTHING
  }

  /**
   * Execute http request. Implementation of &id:oatpp::curl::ProxyRequestExecutor::execute; method.
   * @param method - method ex: ["GET", "POST", "PUT", etc.].
   * @param path - path to resource.
   * @param userDefinedHeaders - headers map &id:oatpp::web::client::RequestExecutor::Headers;.
   * @param body - `std::shared_ptr` to &id:oatpp::web::client::RequestExecutor::Body; object.
   * @param connectionHandle - `nullptr`. This parameter is ignored for now.
   * @return - &id:oatpp::web::protocol::http::incoming::Response;.
   */
  std::shared_ptr<Response> executeOnce(const String &method,
                                        const String &path,
                                        const Headers &userDefinedHeaders,
                                        const std::shared_ptr<Body> &body,
                                        const std::shared_ptr<ConnectionHandle> &connectionHandle = nullptr) override;

  /**
   * Same as &l:ProxyRequestExecutor::execute (); but Async.
   * @param method - method ex: ["GET", "POST", "PUT", etc.].
   * @param path - path to resource.
   * @param headers - headers map &l:RequestExecutor::Headers;.
   * @param body - `std::shared_ptr` to &l:RequestExecutor::Body; object.
   * @param connectionHandle - `nullptr`. This parameter is ignored for now.
   * @return - &id:oatpp::async::CoroutineStarterForResult;.
   */
  virtual oatpp::async::CoroutineStarterForResult<const std::shared_ptr<Response>&>
  executeOnceAsync(const String& method,
                   const String& path,
                   const Headers& headers,
                   const std::shared_ptr<Body>& body,
                   const std::shared_ptr<ConnectionHandle>& connectionHandle = nullptr) override;
  
};
  
}}

#endif /* oatpp_curl_RequestExecutor_hpp */
