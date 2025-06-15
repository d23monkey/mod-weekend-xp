SET @STRING_ENTRY := 11120;
DELETE FROM `acore_string` WHERE `entry` IN  (@STRING_ENTRY+0,@STRING_ENTRY+1,@STRING_ENTRY+2);
INSERT INTO `acore_string` (`entry`, `content_default`, `locale_zhCN`) VALUES
(@STRING_ENTRY+0, 'Your experience rates were set to {}.', '您的经验倍数被设置为 {}.'),
(@STRING_ENTRY+1, 'Wrong value specified. Please specify a value between 0 and {}', '设置的数值不正确。请设置一个介于0和{}之间的值。'),
(@STRING_ENTRY+2, 'The rate being applied to you is {}.\nThe current weekendxp configuration is:\nAnnounce {}\nAlwaysEnabled {}\nQuestOnly {}\nMaxLevel {}\nxpAmount {}\nIndividualXPEnabled {}\nEnabled {}\nMaxAllowedRate {}', '应用到您的倍率是 {}。\n当前的周末经验配置如下：\n公告：{}\n始终启用：{}\n仅限任务：{}\n最大等级：{}\n经验值：{}\n个人经验启用：{}\n启用：{}\n允许的最大倍率：{}');

DELETE FROM `command` WHERE `name` IN ('weekendxp rate');
INSERT INTO `command` (`name`, `security`, `help`) VALUES
('weekendxp rate', 0, 'Syntax: weekendxp rate $value \nSet your experience rate up to the allowed value.');

DELETE FROM `command` WHERE `name` IN ('weekendxp config');
INSERT INTO `command` (`name`, `security`, `help`) VALUES
('weekendxp config', 0, 'Syntax: weekendxp config\nDisplays the current configuration for the weekendxp mod.');
