drop database if exists dbtrain_test_lab2;

use dbtrain_test_lab2;

create table persons(id int, first_name varchar(20), last_name varchar(20), temperature float);

insert into persons values (101, '12345678901234567890', '09876543210987654321', 36.3), (102, '12345678901234567890', '09876543210987654321', 36.3), (103, '12345678901234567890', '09876543210987654321', 36.3), (104, '12345678901234567890', '09876543210987654321', 36.3), (105, '12345678901234567890', '09876543210987654321', 36.3), (106, '12345678901234567890', '09876543210987654321', 36.3), (107, '12345678901234567890', '09876543210987654321', 36.3), (108, '12345678901234567890', '09876543210987654321', 36.3), (109, '12345678901234567890', '09876543210987654321', 36.3), (110, '12345678901234567890', '09876543210987654321', 36.3), (111, '12345678901234567890', '09876543210987654321', 36.3), (112, '12345678901234567890', '09876543210987654321', 36.3), (113, '12345678901234567890', '09876543210987654321', 36.3), (114, '12345678901234567890', '09876543210987654321', 36.3), (115, '12345678901234567890', '09876543210987654321', 36.3), (116, '12345678901234567890', '09876543210987654321', 36.3), (117, '12345678901234567890', '09876543210987654321', 36.3), (118, '12345678901234567890', '09876543210987654321', 36.3), (119, '12345678901234567890', '09876543210987654321', 36.3), (120, '12345678901234567890', '09876543210987654321', 36.3), (121, '12345678901234567890', '09876543210987654321', 36.3), (122, '12345678901234567890', '09876543210987654321', 36.3), (123, '12345678901234567890', '09876543210987654321', 36.3), (124, '12345678901234567890', '09876543210987654321', 36.3), (125, '12345678901234567890', '09876543210987654321', 36.3), (126, '12345678901234567890', '09876543210987654321', 36.3), (127, '12345678901234567890', '09876543210987654321', 36.3), (128, '12345678901234567890', '09876543210987654321', 36.3), (129, '12345678901234567890', '09876543210987654321', 36.3), (130, '12345678901234567890', '09876543210987654321', 36.3), (131, '12345678901234567890', '09876543210987654321', 36.3), (132, '12345678901234567890', '09876543210987654321', 36.3), (133, '12345678901234567890', '09876543210987654321', 36.3), (134, '12345678901234567890', '09876543210987654321', 36.3), (135, '12345678901234567890', '09876543210987654321', 36.3), (136, '12345678901234567890', '09876543210987654321', 36.3), (137, '12345678901234567890', '09876543210987654321', 36.3), (138, '12345678901234567890', '09876543210987654321', 36.3), (139, '12345678901234567890', '09876543210987654321', 36.3), (140, '12345678901234567890', '09876543210987654321', 36.3), (141, '12345678901234567890', '09876543210987654321', 36.3), (142, '12345678901234567890', '09876543210987654321', 36.3), (143, '12345678901234567890', '09876543210987654321', 36.3), (144, '12345678901234567890', '09876543210987654321', 36.3), (145, '12345678901234567890', '09876543210987654321', 36.3), (146, '12345678901234567890', '09876543210987654321', 36.3), (147, '12345678901234567890', '09876543210987654321', 36.3), (148, '12345678901234567890', '09876543210987654321', 36.3), (149, '12345678901234567890', '09876543210987654321', 36.3), (150, '12345678901234567890', '09876543210987654321', 36.3), (151, '12345678901234567890', '09876543210987654321', 36.3), (152, '12345678901234567890', '09876543210987654321', 36.3), (153, '12345678901234567890', '09876543210987654321', 36.3), (154, '12345678901234567890', '09876543210987654321', 36.3), (155, '12345678901234567890', '09876543210987654321', 36.3), (156, '12345678901234567890', '09876543210987654321', 36.3), (157, '12345678901234567890', '09876543210987654321', 36.3), (158, '12345678901234567890', '09876543210987654321', 36.3), (159, '12345678901234567890', '09876543210987654321', 36.3), (160, '12345678901234567890', '09876543210987654321', 36.3), (161, '12345678901234567890', '09876543210987654321', 36.3), (162, '12345678901234567890', '09876543210987654321', 36.3), (163, '12345678901234567890', '09876543210987654321', 36.3), (164, '12345678901234567890', '09876543210987654321', 36.3), (165, '12345678901234567890', '09876543210987654321', 36.3), (166, '12345678901234567890', '09876543210987654321', 36.3), (167, '12345678901234567890', '09876543210987654321', 36.3), (168, '12345678901234567890', '09876543210987654321', 36.3), (169, '12345678901234567890', '09876543210987654321', 36.3), (170, '12345678901234567890', '09876543210987654321', 36.3), (171, '12345678901234567890', '09876543210987654321', 36.3), (172, '12345678901234567890', '09876543210987654321', 36.3), (173, '12345678901234567890', '09876543210987654321', 36.3), (174, '12345678901234567890', '09876543210987654321', 36.3), (175, '12345678901234567890', '09876543210987654321', 36.3), (176, '12345678901234567890', '09876543210987654321', 36.3), (177, '12345678901234567890', '09876543210987654321', 36.3), (178, '12345678901234567890', '09876543210987654321', 36.3), (179, '12345678901234567890', '09876543210987654321', 36.3), (180, '12345678901234567890', '09876543210987654321', 36.3), (181, '12345678901234567890', '09876543210987654321', 36.3), (182, '12345678901234567890', '09876543210987654321', 36.3), (183, '12345678901234567890', '09876543210987654321', 36.3), (184, '12345678901234567890', '09876543210987654321', 36.3), (185, '12345678901234567890', '09876543210987654321', 36.3), (186, '12345678901234567890', '09876543210987654321', 36.3), (187, '12345678901234567890', '09876543210987654321', 36.3), (188, '12345678901234567890', '09876543210987654321', 36.3), (189, '12345678901234567890', '09876543210987654321', 36.3), (190, '12345678901234567890', '09876543210987654321', 36.3), (191, '12345678901234567890', '09876543210987654321', 36.3), (192, '12345678901234567890', '09876543210987654321', 36.3), (193, '12345678901234567890', '09876543210987654321', 36.3), (194, '12345678901234567890', '09876543210987654321', 36.3), (195, '12345678901234567890', '09876543210987654321', 36.3), (196, '12345678901234567890', '09876543210987654321', 36.3), (197, '12345678901234567890', '09876543210987654321', 36.3), (198, '12345678901234567890', '09876543210987654321', 36.3), (199, '12345678901234567890', '09876543210987654321', 36.3), (200, '12345678901234567890', '09876543210987654321', 36.3);
insert into persons values (1, '12345678901234567890', '09876543210987654321', 36.3), (2, '12345678901234567890', '09876543210987654321', 36.3), (3, '12345678901234567890', '09876543210987654321', 36.3), (4, '12345678901234567890', '09876543210987654321', 36.3), (5, '12345678901234567890', '09876543210987654321', 36.3), (6, '12345678901234567890', '09876543210987654321', 36.3), (7, '12345678901234567890', '09876543210987654321', 36.3), (8, '12345678901234567890', '09876543210987654321', 36.3), (9, '12345678901234567890', '09876543210987654321', 36.3), (10, '12345678901234567890', '09876543210987654321', 36.3), (11, '12345678901234567890', '09876543210987654321', 36.3), (12, '12345678901234567890', '09876543210987654321', 36.3), (13, '12345678901234567890', '09876543210987654321', 36.3), (14, '12345678901234567890', '09876543210987654321', 36.3), (15, '12345678901234567890', '09876543210987654321', 36.3), (16, '12345678901234567890', '09876543210987654321', 36.3), (17, '12345678901234567890', '09876543210987654321', 36.3), (18, '12345678901234567890', '09876543210987654321', 36.3), (19, '12345678901234567890', '09876543210987654321', 36.3), (20, '12345678901234567890', '09876543210987654321', 36.3), (21, '12345678901234567890', '09876543210987654321', 36.3), (22, '12345678901234567890', '09876543210987654321', 36.3), (23, '12345678901234567890', '09876543210987654321', 36.3), (24, '12345678901234567890', '09876543210987654321', 36.3), (25, '12345678901234567890', '09876543210987654321', 36.3), (26, '12345678901234567890', '09876543210987654321', 36.3), (27, '12345678901234567890', '09876543210987654321', 36.3), (28, '12345678901234567890', '09876543210987654321', 36.3), (29, '12345678901234567890', '09876543210987654321', 36.3), (30, '12345678901234567890', '09876543210987654321', 36.3), (31, '12345678901234567890', '09876543210987654321', 36.3), (32, '12345678901234567890', '09876543210987654321', 36.3), (33, '12345678901234567890', '09876543210987654321', 36.3), (34, '12345678901234567890', '09876543210987654321', 36.3), (35, '12345678901234567890', '09876543210987654321', 36.3), (36, '12345678901234567890', '09876543210987654321', 36.3), (37, '12345678901234567890', '09876543210987654321', 36.3), (38, '12345678901234567890', '09876543210987654321', 36.3), (39, '12345678901234567890', '09876543210987654321', 36.3), (40, '12345678901234567890', '09876543210987654321', 36.3), (41, '12345678901234567890', '09876543210987654321', 36.3), (42, '12345678901234567890', '09876543210987654321', 36.3), (43, '12345678901234567890', '09876543210987654321', 36.3), (44, '12345678901234567890', '09876543210987654321', 36.3), (45, '12345678901234567890', '09876543210987654321', 36.3), (46, '12345678901234567890', '09876543210987654321', 36.3), (47, '12345678901234567890', '09876543210987654321', 36.3), (48, '12345678901234567890', '09876543210987654321', 36.3), (49, '12345678901234567890', '09876543210987654321', 36.3), (50, '12345678901234567890', '09876543210987654321', 36.3), (51, '12345678901234567890', '09876543210987654321', 36.3), (52, '12345678901234567890', '09876543210987654321', 36.3), (53, '12345678901234567890', '09876543210987654321', 36.3), (54, '12345678901234567890', '09876543210987654321', 36.3), (55, '12345678901234567890', '09876543210987654321', 36.3), (56, '12345678901234567890', '09876543210987654321', 36.3), (57, '12345678901234567890', '09876543210987654321', 36.3), (58, '12345678901234567890', '09876543210987654321', 36.3), (59, '12345678901234567890', '09876543210987654321', 36.3), (60, '12345678901234567890', '09876543210987654321', 36.3), (61, '12345678901234567890', '09876543210987654321', 36.3), (62, '12345678901234567890', '09876543210987654321', 36.3), (63, '12345678901234567890', '09876543210987654321', 36.3), (64, '12345678901234567890', '09876543210987654321', 36.3), (65, '12345678901234567890', '09876543210987654321', 36.3), (66, '12345678901234567890', '09876543210987654321', 36.3), (67, '12345678901234567890', '09876543210987654321', 36.3), (68, '12345678901234567890', '09876543210987654321', 36.3), (69, '12345678901234567890', '09876543210987654321', 36.3), (70, '12345678901234567890', '09876543210987654321', 36.3), (71, '12345678901234567890', '09876543210987654321', 36.3), (72, '12345678901234567890', '09876543210987654321', 36.3), (73, '12345678901234567890', '09876543210987654321', 36.3), (74, '12345678901234567890', '09876543210987654321', 36.3), (75, '12345678901234567890', '09876543210987654321', 36.3), (76, '12345678901234567890', '09876543210987654321', 36.3), (77, '12345678901234567890', '09876543210987654321', 36.3), (78, '12345678901234567890', '09876543210987654321', 36.3), (79, '12345678901234567890', '09876543210987654321', 36.3), (80, '12345678901234567890', '09876543210987654321', 36.3), (81, '12345678901234567890', '09876543210987654321', 36.3), (82, '12345678901234567890', '09876543210987654321', 36.3), (83, '12345678901234567890', '09876543210987654321', 36.3), (84, '12345678901234567890', '09876543210987654321', 36.3), (85, '12345678901234567890', '09876543210987654321', 36.3), (86, '12345678901234567890', '09876543210987654321', 36.3), (87, '12345678901234567890', '09876543210987654321', 36.3), (88, '12345678901234567890', '09876543210987654321', 36.3), (89, '12345678901234567890', '09876543210987654321', 36.3), (90, '12345678901234567890', '09876543210987654321', 36.3), (91, '12345678901234567890', '09876543210987654321', 36.3), (92, '12345678901234567890', '09876543210987654321', 36.3), (93, '12345678901234567890', '09876543210987654321', 36.3), (94, '12345678901234567890', '09876543210987654321', 36.3), (95, '12345678901234567890', '09876543210987654321', 36.3), (96, '12345678901234567890', '09876543210987654321', 36.3), (97, '12345678901234567890', '09876543210987654321', 36.3), (98, '12345678901234567890', '09876543210987654321', 36.3), (99, '12345678901234567890', '09876543210987654321', 36.3), (100, '12345678901234567890', '09876543210987654321', 36.3);
