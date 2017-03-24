#include "matrixuser.h"

String MatrixUser::get_display_name(bool sync) {
  if (sync) {
    Dictionary response;
    client->request_json("/_matrix/client/r0/profile/"+user_id+"/displayname", Dictionary(), HTTPClient::Method::METHOD_GET, response);
    display_name = response["displayname"];
  }

  return display_name;
}

String MatrixUser::get_friendly_name(bool sync) {
  get_display_name(sync);

  if (display_name.length() != 0) {
    return display_name;
  } else {
    return user_id;
  }
}

Error MatrixUser::set_display_name(String name) {
  Dictionary request_body;
  request_body["displayname"] = name;

  Dictionary response;
  HTTPClient::ResponseCode http_status = client->request_json("/_matrix/client/r0/profile/"+user_id+"/displayname", request_body, HTTPClient::Method::METHOD_PUT, response);

  if (http_status == 200) {
    display_name = name;
    return Error::OK;
  } else {
    WARN_PRINT(((String)response["error"]).utf8().get_data());
    return Error::ERR_QUERY_FAILED;
  }
}

String MatrixUser::get_avatar_url(bool sync) {
  if (sync) {
    Dictionary response;
    HTTPClient::ResponseCode http_status = client->request_json("/_matrix/client/r0/profile/"+user_id+"/displayname", Dictionary(), HTTPClient::Method::METHOD_GET, response);
    if (response.has("avatar_url")) {
      avatar_url = response["avatar_url"];
    } else {
      avatar_url = String();
    }
  }

  return "https://"+client->get_hs_name()+"/_matrix/media/r0/download/"+avatar_url.substr(6, avatar_url.length()-6);
}

Error MatrixUser::set_avatar_url(String mxcurl) {
  Dictionary request_body;
  request_body["avatar_url"] = mxcurl;

  Dictionary response;
  HTTPClient::ResponseCode http_status = client->request_json("/_matrix/client/r0/profile/"+user_id+"/avatar_url", request_body, HTTPClient::Method::METHOD_PUT, response);

  if (http_status == 200) {
    avatar_url = mxcurl;
    return Error::OK;
  } else {
    WARN_PRINT(((String)response["error"]).utf8().get_data());
    return Error::ERR_QUERY_FAILED;
  }
}

MatrixUser::MatrixUser() {
}

void MatrixUser::init(MatrixClient *c, String id) {
  client = c;
  user_id = id;

}
void MatrixUser::_bind_methods() {
}
