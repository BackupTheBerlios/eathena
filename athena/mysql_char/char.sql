# database : `ragnarok`
# 

# --------------------------------------------------------

#
# table structure `cart_inventory`
#

CREATE TABLE `cart_inventory` (
  `id` int(11) NOT NULL default '0',
  `char_id` int(11) NOT NULL default '0',
  `nameid` int(11) NOT NULL default '0',
  `amount` int(11) NOT NULL default '0',
  `equip` mediumint(8) unsigned NOT NULL default '0',
  `identify` smallint(6) NOT NULL default '0',
  `refine` tinyint(3) unsigned NOT NULL default '0',
  `attribute` tinyint(4) NOT NULL default '0',
  `card0` int(11) NOT NULL default '0',
  `card1` int(11) NOT NULL default '0',
  `card2` int(11) NOT NULL default '0',
  `card3` int(11) NOT NULL default '0',
  KEY `char_id` (`char_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# table structure `char`
#
CREATE TABLE `char` (
  `char_id` int(11) NOT NULL auto_increment,
  `account_id` int(11) NOT NULL default '0',
  `char_num` tinyint(4) NOT NULL default '0',
  `name` varchar(255) NOT NULL default '',
  `class` tinyint(4) NOT NULL default '0',
  `base_level` tinyint(4) NOT NULL default '1',
  `job_level` tinyint(4) NOT NULL default '1',
  `base_exp` int(11) NOT NULL default '0',
  `job_exp` int(11) NOT NULL default '0',
  `zeny` int(11) NOT NULL default '500',
  `str` tinyint(4) NOT NULL default '0',
  `agi` tinyint(4) NOT NULL default '0',
  `vit` tinyint(4) NOT NULL default '0',
  `int` tinyint(4) NOT NULL default '0',
  `dex` tinyint(4) NOT NULL default '0',
  `luk` tinyint(4) NOT NULL default '0',
  `max_hp` int(11) NOT NULL default '0',
  `hp` int(11) NOT NULL default '0',
  `max_sp` int(11) NOT NULL default '0',
  `sp` int(11) NOT NULL default '0',
  `status_point` int(11) NOT NULL default '0',
  `skill_point` int(11) NOT NULL default '0',
  `option` int(11) NOT NULL default '0',
  `karma` int(11) NOT NULL default '0',
  `manner` int(11) NOT NULL default '0',
  `party_id` int(11) NOT NULL default '0',
  `guild_id` int(11) NOT NULL default '0',
  `pet_id` int(11) NOT NULL default '0',
  `hair` tinyint(4) NOT NULL default '0',
  `hair_color` int(11) NOT NULL default '0',
  `clothes_color` tinyint(4) NOT NULL default '0',
  `weapon` int(11) NOT NULL default '1',
  `shield` int(11) NOT NULL default '0',
  `head_top` int(11) NOT NULL default '0',
  `head_mid` int(11) NOT NULL default '0',
  `head_bottom` int(11) NOT NULL default '0',
  `last_map` varchar(20) NOT NULL default 'new_1-1.gat',
  `last_x` int(4) NOT NULL default '53',
  `last_y` int(11) NOT NULL default '111',
  `save_map` varchar(20) NOT NULL default 'new_1-1.gat',
  `save_x` int(11) NOT NULL default '53',
  `save_y` int(11) NOT NULL default '111',
  PRIMARY KEY  (`char_id`)
) TYPE=MyISAM AUTO_INCREMENT=100000 ;

# --------------------------------------------------------

#
# table structure `global_reg_value`
#

CREATE TABLE `global_reg_value` (
  `char_id` int(11) NOT NULL default '0',
  `str` varchar(255) NOT NULL default '',
  `value` int(11) NOT NULL default '0',
  KEY `char_id` (`char_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# table structure `inventory`
#

CREATE TABLE `inventory` (
  `id` int(11) NOT NULL default '0',
  `char_id` int(11) NOT NULL default '0',
  `nameid` int(11) NOT NULL default '0',
  `amount` int(11) NOT NULL default '0',
  `equip` mediumint(8) unsigned NOT NULL default '0',
  `identify` smallint(6) NOT NULL default '0',
  `refine` tinyint(3) unsigned NOT NULL default '0',
  `attribute` tinyint(4) NOT NULL default '0',
  `card0` int(11) NOT NULL default '0',
  `card1` int(11) NOT NULL default '0',
  `card2` int(11) NOT NULL default '0',
  `card3` int(11) NOT NULL default '0',
  KEY `char_id` (`char_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# table structure `memo`
#

CREATE TABLE `memo` (
  `memo_id` int(11) NOT NULL auto_increment,
  `char_id` int(11) NOT NULL default '0',
  `map` varchar(255) NOT NULL default '',
  `x` mediumint(9) NOT NULL default '0',
  `y` mediumint(9) NOT NULL default '0',
  PRIMARY KEY  (`memo_id`)
) TYPE=MyISAM AUTO_INCREMENT=1 ;

# --------------------------------------------------------

#
# table structure `pet`
#

CREATE TABLE `pet` (
  `pet_id` int(11) NOT NULL auto_increment,
  `class` mediumint(9) NOT NULL default '0',
  `name` varchar(24) NOT NULL default '',
  `account_id` int(11) NOT NULL default '0',
  `char_id` int(11) NOT NULL default '0',
  `level` tinyint(4) NOT NULL default '0',
  `egg_id` int(11) NOT NULL default '0',
  `equip` mediumint(8) unsigned NOT NULL default '0',
  `intimate` mediumint(9) NOT NULL default '0',
  `hungry` mediumint(9) NOT NULL default '0',
  `rename_flag` tinyint(4) NOT NULL default '0',
  `incuvate` int(11) NOT NULL default '0',
  PRIMARY KEY  (`pet_id`)
) TYPE=MyISAM AUTO_INCREMENT=10000 ;

# --------------------------------------------------------

#
# table structure `skill`
#

CREATE TABLE `skill` (
  `char_id` int(11) NOT NULL default '0',
  `id` int(11) NOT NULL default '0',
  `lv` tinyint(4) NOT NULL default '0',
  KEY `char_id` (`char_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# table structure `storage`
#

CREATE TABLE `storage` (
  `account_id` int(11) NOT NULL default '0',
  `id` int(11) NOT NULL default '0',
  `nameid` int(11) NOT NULL default '0',
  `amount` int(11) NOT NULL default '0',
  `equip` mediumint(8) unsigned NOT NULL default '0',
  `identify` smallint(6) NOT NULL default '0',
  `refine` tinyint(3) unsigned NOT NULL default '0',
  `attribute` tinyint(4) NOT NULL default '0',
  `card0` int(11) NOT NULL default '0',
  `card1` int(11) NOT NULL default '0',
  `card2` int(11) NOT NULL default '0',
  `card3` int(11) NOT NULL default '0',
  KEY `account_id` (`account_id`)
) TYPE=MyISAM;
