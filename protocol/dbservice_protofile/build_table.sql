-- Role data : base info
DROP TABLE IF EXISTS `role_data`;
CREATE TABLE `role_data` (
    `id`                bigint NOT NULL AUTO_INCREMENT COMMENT 'Primary key, should auto increment.',
    `role_id`           bigint(20)      NOT NULL DEFAULT 0,
    `player_name`       varchar(64)     NOT NULL DEFAULT '',
    `account`           varchar(128)    NOT NULL DEFAULT '',
    `equip_base_bin`    mediumblob,         
    `equip_ext_bin`     mediumblob,
    `item_bin`          mediumblob,
    `pet_bin`           mediumblob,
    `created_time`      datetime        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `updated_time`      datetime        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY(`id`),
    KEY(`role_id`)
) ENGINE = InnoDB AUTO_INCREMENT 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- Global data : activity1
DROP TABLE IF EXISTS `activity_1`;
CREATE TABLE `activity_1` (
    `id`                bigint          NOT NULL AUTO_INCREMENT COMMENT 'Primary key, should auto increment.',
    `activity_data_id`  bigint(20)      NOT NULL DEFAULT 0,
    `role_id`           bigint(20)      NOT NULL DEFAULT 0,
    `activity_bin`      mediumblob,
    `created_time`      datetime        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `updated_time`      datetime        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY(`id`)
) ENGINE = InnoDB AUTO_INCREMENT 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;