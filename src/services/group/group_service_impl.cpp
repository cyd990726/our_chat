#include "services/group_service_impl.h"
#include "common/logger.h"

namespace ourchat {

grpc::Status GroupServiceImpl::CreateGroup(grpc::ServerContext* context,
                                            const im::CreateGroupRequest* request,
                                            im::CreateGroupResponse* response) {
    LOG_INFO("CreateGroup: name=" + request->group_name());
    response->set_success(true);
    response->set_group_id(1001);
    return grpc::Status::OK;
}

grpc::Status GroupServiceImpl::GetGroupInfo(grpc::ServerContext* context,
                                             const im::GetGroupInfoRequest* request,
                                             im::GetGroupInfoResponse* response) {
    LOG_INFO("GetGroupInfo: group_id=" + std::to_string(request->group_id()));
    return grpc::Status::OK;
}

}
