--
-- Drop tables for cleanup
--

DROP TABLE IF EXISTS `utilizatori`;
DROP TABLE IF EXISTS `asocieri`;
DROP TABLE IF EXISTS `universitati`;
DROP TABLE IF EXISTS `vacante`;
DROP TABLE IF EXISTS `facultati`;
DROP TABLE IF EXISTS `catedre`;
DROP TABLE IF EXISTS `discipline`;
DROP TABLE IF EXISTS `titulari`;
DROP TABLE IF EXISTS `ore`;


-- 
-- Table structure for table `universitati`
-- 

CREATE TABLE `universitati` (
  `univ_id` int(11) NOT NULL auto_increment,
  `nume` varchar(50) NOT NULL default '',
  `data_start` date NOT NULL default '0000-00-00',
  `data_stop` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`univ_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- 
-- Table structure for table `vacante`
-- 

CREATE TABLE `vacante` (
  `vac_id` int(11) NOT NULL auto_increment,
  `link_univ` int(11) NOT NULL default '0',
  `data_start` date NOT NULL default '0000-00-00',
  `data_stop` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`vac_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- 
-- Table structure for table `facultati`
-- 

CREATE TABLE `facultati` (
  `fac_id` int(11) NOT NULL auto_increment,
  `link_univ` int(11) NOT NULL default '0',
  `nume` varchar(80) NOT NULL default '',
  `nume_scurt` varchar(10) NOT NULL default '',
  `decan` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`fac_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- 
-- Table structure for table `catedre`
-- 

CREATE TABLE `catedre` (
  `cat_id` int(11) NOT NULL auto_increment,
  `link_fac` int(11) NOT NULL default '0',
  `nume` varchar(50) NOT NULL default '',
  `sef` varchar(35) NOT NULL default '',
  PRIMARY KEY  (`cat_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Table structure for table `discipline`
--

CREATE TABLE `discipline` (
  `disc_id` int(11) NOT NULL auto_increment,
  `link_fac` int(11) NOT NULL default '0',
  `link_cat` int(11) NOT NULL default '0',
  `nume` varchar(50) NOT NULL default '',
  `nume_scurt` varchar(10) NOT NULL default '',
  PRIMARY KEY (`disc_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `titulari`
--

CREATE TABLE `titulari` (
  `tit_id` int(11) NOT NULL auto_increment,
  `link_disc` int(11) NOT NULL default '0',
  `nume` varchar(50) NOT NULL default '',
  `an` tinyint(4) NOT NULL default '0',
  `serie` varchar(5) NOT NULL default '',
  PRIMARY KEY (`tit_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- 
-- Table structure for table `utilizatori`
--
-- tip_cont: 'S' for secretary, 'P' for professor, 'A' for administrator
-- 

CREATE TABLE `utilizatori` (
  `utilizator_id` int(11) NOT NULL auto_increment,
  `utilizator` varchar(40) NOT NULL default '',
  `parola` varchar(40) NOT NULL default '',
  `nume` varchar(40) NOT NULL default '',
  `email` varchar(45) NOT NULL default '',
  `tip_cont` varchar(5) NOT NULL default '',
  `link_cat` int(11) NOT NULL default '0',
  `link_disc` int(11) NOT NULL default '0',
  PRIMARY KEY  (`utilizator_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- 
-- Table structure for table `ore`
-- 

CREATE TABLE `ore` (
  `ora_id` int(11) NOT NULL auto_increment,
  `link_disc` int(11) NOT NULL default '0',
  `tip_ora` varchar(10) NOT NULL default '',
  `forma` varchar(5) NOT NULL default '',
  `cod` varchar(6) NOT NULL default '',
  `an` tinyint(1) NOT NULL default '0',
  `serie` varchar(5) NOT NULL default '',
  `nr_stud` int(5) NOT NULL default '0',
  `nr_grupa` tinyint(3) NOT NULL default '0',
  `tip_grupa_aplicatii` varchar(4) NOT NULL default '',
  `nr_ore_curs` tinyint(3) NOT NULL default '0',
  `nr_ore_aplicatii` tinyint(3) NOT NULL default '0',
  `nr_post` int(5) NOT NULL default '0',
  `grad_post` varchar(5) NOT NULL default '',
  `pers_norma` varchar(30) NOT NULL default '',
  `tip_ocupare` varchar(5) NOT NULL default '',
  `pers_acoperit` varchar(30) NOT NULL default '',
  `pers_acoperit_efect` varchar(30) NOT NULL default '',
  `an_grupa` varchar(12) NOT NULL default '',
  `zi` varchar(10) NOT NULL default '',
  `ora` varchar(8) NOT NULL default '',
  `sala` varchar(10) NOT NULL default '',
  `paritate` tinyint(1) NOT NULL default '1',
  `paritate_start` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`ora_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;
