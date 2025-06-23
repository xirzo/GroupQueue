CREATE TABLE list_user(
    list_user_id bigserial PRIMARY KEY,
    list_id bigint,
    user_id bigint,
    user_order bigint
);
