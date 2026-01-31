#pragma once
#include "group.pb.h"
#include <grpcpp/grpcpp.h>

namespace im {

class GroupService {
public:
    virtual ~GroupService() = default;

    virtual grpc::Status CreateGroup(grpc::ServerContext* context,
                                     const CreateGroupRequest* request,
                                     CreateGroupResponse* response) = 0;

    virtual grpc::Status GetGroupInfo(grpc::ServerContext* context,
                                      const GetGroupInfoRequest* request,
                                      GetGroupInfoResponse* response) = 0;
};

} // namespace im
