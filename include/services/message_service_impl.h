#ifndef MESSAGE_SERVICE_IMPL_H
#define MESSAGE_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include "message.grpc.pb.h"

namespace ourchat {

class MessageServiceImpl : public im::MessageService, public grpc::Service {
public:
    grpc::Status SendMessage(grpc::ServerContext* context,
                             const im::SendMessageRequest* request,
                             im::SendMessageResponse* response) override;
    
    grpc::Status GetMessages(grpc::ServerContext* context,
                             const im::GetMessagesRequest* request,
                             im::GetMessagesResponse* response) override;
};

}

#endif
