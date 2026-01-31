#pragma once
#include "message.pb.h"
#include <grpcpp/grpcpp.h>

namespace im {

class MessageService {
public:
    virtual ~MessageService() = default;

    virtual grpc::Status SendMessage(grpc::ServerContext* context,
                                     const SendMessageRequest* request,
                                     SendMessageResponse* response) = 0;

    virtual grpc::Status GetMessages(grpc::ServerContext* context,
                                     const GetMessagesRequest* request,
                                     GetMessagesResponse* response) = 0;
};

} // namespace im
