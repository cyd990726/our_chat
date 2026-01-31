#include "services/message_service_impl.h"
#include "common/logger.h"

namespace ourchat {

grpc::Status MessageServiceImpl::SendMessage(grpc::ServerContext* context,
                                              const im::SendMessageRequest* request,
                                              im::SendMessageResponse* response) {
    LOG_INFO("SendMessage: from=" + std::to_string(request->sender_id()) + 
             " to=" + std::to_string(request->receiver_id()));
    
    response->set_success(true);
    response->set_message_id(1001);
    
    return grpc::Status::OK;
}

grpc::Status MessageServiceImpl::GetMessages(grpc::ServerContext* context,
                                              const im::GetMessagesRequest* request,
                                              im::GetMessagesResponse* response) {
    LOG_INFO("GetMessages: user_id=" + std::to_string(request->user_id()));
    
    return grpc::Status::OK;
}

}
