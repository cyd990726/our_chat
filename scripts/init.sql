-- OurChat Database Initialization Script
-- Database: ourchat_db
-- Charset: utf8mb4
-- Collation: utf8mb4_unicode_ci

-- Create database if not exists
CREATE DATABASE IF NOT EXISTS ourchat_db 
    DEFAULT CHARACTER SET utf8mb4 
    DEFAULT COLLATE utf8mb4_unicode_ci;

USE ourchat_db;

-- User table
CREATE TABLE IF NOT EXISTS im_user (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(64) NOT NULL UNIQUE COMMENT '用户名',
    phone VARCHAR(20) UNIQUE COMMENT '手机号',
    email VARCHAR(128) UNIQUE COMMENT '邮箱',
    password_hash VARCHAR(128) NOT NULL COMMENT '密码哈希',
    avatar_url VARCHAR(256) DEFAULT '' COMMENT '头像URL',
    nickname VARCHAR(64) DEFAULT '' COMMENT '昵称',
    status TINYINT DEFAULT 1 COMMENT '状态: 1-正常, 2-禁用',
    create_time BIGINT UNSIGNED NOT NULL COMMENT '创建时间',
    update_time BIGINT UNSIGNED NOT NULL COMMENT '更新时间',
    INDEX idx_phone(phone),
    INDEX idx_create_time(create_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='用户表';

-- Friend relationship table
CREATE TABLE IF NOT EXISTS im_friend (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
    friend_id BIGINT UNSIGNED NOT NULL COMMENT '好友ID',
    alias VARCHAR(64) DEFAULT '' COMMENT '好友备注',
    status TINYINT DEFAULT 1 COMMENT '状态: 1-正常, 2-已删除',
    create_time BIGINT UNSIGNED NOT NULL COMMENT '创建时间',
    UNIQUE KEY uk_user_friend(user_id, friend_id),
    INDEX idx_friend_id(friend_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='好友关系表';

-- Session table
CREATE TABLE IF NOT EXISTS im_session (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
    peer_id BIGINT UNSIGNED NOT NULL COMMENT '对方ID',
    session_type TINYINT NOT NULL COMMENT '会话类型: 1-单聊, 2-群聊',
    last_message_id BIGINT UNSIGNED DEFAULT NULL COMMENT '最后消息ID',
    last_message_content VARCHAR(512) DEFAULT '' COMMENT '最后消息内容',
    last_message_time BIGINT UNSIGNED NOT NULL COMMENT '最后消息时间',
    unread_count INT DEFAULT 0 COMMENT '未读消息数',
    INDEX idx_user_session(user_id, last_message_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='会话表';

-- Group table
CREATE TABLE IF NOT EXISTS im_group (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    group_name VARCHAR(128) NOT NULL COMMENT '群名称',
    group_avatar VARCHAR(256) DEFAULT '' COMMENT '群头像',
    owner_id BIGINT UNSIGNED NOT NULL COMMENT '群主ID',
    description VARCHAR(512) DEFAULT '' COMMENT '群描述',
    member_count INT UNSIGNED DEFAULT 0 COMMENT '成员数量',
    max_members INT UNSIGNED DEFAULT 500 COMMENT '最大成员数',
    status TINYINT DEFAULT 1 COMMENT '状态: 1-正常, 2-已解散',
    create_time BIGINT UNSIGNED NOT NULL COMMENT '创建时间',
    update_time BIGINT UNSIGNED NOT NULL COMMENT '更新时间',
    INDEX idx_owner(owner_id),
    INDEX idx_create_time(create_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='群组表';

-- Group member table
CREATE TABLE IF NOT EXISTS im_group_member (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    group_id BIGINT UNSIGNED NOT NULL COMMENT '群组ID',
    user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
    role TINYINT DEFAULT 1 COMMENT '角色: 1-普通成员, 2-管理员, 3-群主',
    alias VARCHAR(64) DEFAULT '' COMMENT '群内备注',
    join_time BIGINT UNSIGNED NOT NULL COMMENT '加入时间',
    UNIQUE KEY uk_group_user(group_id, user_id),
    INDEX idx_user(user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='群成员表';

-- Single chat message table
CREATE TABLE IF NOT EXISTS im_single_message (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    conversation_id BIGINT UNSIGNED NOT NULL COMMENT '会话ID',
    sender_id BIGINT UNSIGNED NOT NULL COMMENT '发送者ID',
    receiver_id BIGINT UNSIGNED NOT NULL COMMENT '接收者ID',
    message_type TINYINT NOT NULL COMMENT '消息类型',
    content TEXT COMMENT '消息内容',
    status TINYINT DEFAULT 1 COMMENT '状态: 1-发送中, 2-已送达, 3-已读',
    create_time BIGINT UNSIGNED NOT NULL COMMENT '创建时间',
    INDEX idx_conversation(conversation_id, create_time),
    INDEX idx_sender(sender_id),
    INDEX idx_receiver(receiver_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='单聊消息表';

-- Group message table
CREATE TABLE IF NOT EXISTS im_group_message (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    group_id BIGINT UNSIGNED NOT NULL COMMENT '群组ID',
    sender_id BIGINT UNSIGNED NOT NULL COMMENT '发送者ID',
    message_type TINYINT NOT NULL COMMENT '消息类型',
    content TEXT COMMENT '消息内容',
    seq_id BIGINT UNSIGNED NOT NULL COMMENT '消息序列号',
    create_time BIGINT UNSIGNED NOT NULL COMMENT '创建时间',
    INDEX idx_group_seq(group_id, seq_id),
    INDEX idx_sender(sender_id),
    INDEX idx_create_time(create_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='群消息表';

-- Message read receipt table
CREATE TABLE IF NOT EXISTS im_message_read (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
    peer_id BIGINT UNSIGNED NOT NULL COMMENT '对方ID',
    last_read_message_id BIGINT UNSIGNED NOT NULL COMMENT '最后已读消息ID',
    last_read_time BIGINT UNSIGNED NOT NULL COMMENT '最后已读时间',
    UNIQUE KEY uk_user_peer(user_id, peer_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='消息已读回执表';

-- Device table
CREATE TABLE IF NOT EXISTS im_device (
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
    device_id VARCHAR(64) NOT NULL UNIQUE COMMENT '设备ID',
    device_type VARCHAR(32) NOT NULL COMMENT '设备类型: ios/android/web',
    device_token VARCHAR(256) DEFAULT '' COMMENT '推送Token',
    last_active_time BIGINT UNSIGNED NOT NULL COMMENT '最后活跃时间',
    status TINYINT DEFAULT 1 COMMENT '状态: 1-在线, 2-离线',
    create_time BIGINT UNSIGNED NOT NULL COMMENT '创建时间',
    INDEX idx_user(user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='设备表';

-- Insert default admin user (password: admin123)
INSERT INTO im_user (username, password_hash, phone, email, nickname, create_time, update_time) 
VALUES ('admin', '$2b$12$LQv3c1yqBWVHxkd0LHAkCOYz6TtxMQJqhN8/X4.DvLDaLdG1pAuFy', 
        '13800138000', 'admin@ourchat.com', '管理员', UNIX_TIMESTAMP(), UNIX_TIMESTAMP());

-- Create indexes for better performance
CREATE INDEX idx_user_username ON im_user(username);
CREATE INDEX idx_user_status ON im_user(status);
CREATE INDEX idx_group_status ON im_group(status);
CREATE INDEX idx_group_member_group ON im_group_member(group_id);
CREATE INDEX idx_single_message_sender ON im_single_message(sender_id, create_time);
CREATE INDEX idx_single_message_receiver ON im_single_message(receiver_id, create_time);
CREATE INDEX idx_group_message_group ON im_group_message(group_id, create_time);
