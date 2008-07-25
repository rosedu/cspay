-- phpMyAdmin SQL Dump
-- version 2.9.1.1-Debian-7
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Jul 25, 2008 at 11:14 AM
-- Server version: 5.0.32
-- PHP Version: 5.2.0-8+etch11
-- 
-- Database: `cspay`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `admin`
-- 

CREATE TABLE `admin` (
  `admin_id` int(11) NOT NULL auto_increment,
  `materie` varchar(40) NOT NULL default '',
  `utilizator` varchar(40) NOT NULL default '',
  `parola` varchar(40) NOT NULL default '',
  `tip_cont` smallint(6) NOT NULL default '0',
  `link_univ` int(11) NOT NULL default '0',
  PRIMARY KEY  (`admin_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=395 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `asociatie`
-- 

CREATE TABLE `asociatie` (
  `asoc_id` int(11) NOT NULL auto_increment,
  `link_admin` int(11) NOT NULL default '0',
  `nume` varchar(40) NOT NULL default '',
  `email` varchar(45) NOT NULL default '',
  PRIMARY KEY  (`asoc_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `catedra`
-- 

CREATE TABLE `catedra` (
  `cat_id` int(11) NOT NULL auto_increment,
  `link_fac` int(11) NOT NULL default '0',
  `nume` varchar(50) NOT NULL default '',
  `sef` varchar(35) NOT NULL default '',
  PRIMARY KEY  (`cat_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=8 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `facultate`
-- 

CREATE TABLE `facultate` (
  `fac_id` int(11) NOT NULL auto_increment,
  `link_univ` int(11) NOT NULL default '0',
  `nume` varchar(50) NOT NULL default '',
  `nume_scurt` varchar(50) NOT NULL default '',
  `decan` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`fac_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `orar`
-- 

CREATE TABLE `orar` (
  `orar_id` int(11) NOT NULL auto_increment,
  `facultate` varchar(35) NOT NULL default '',
  `tip_curs1` varchar(10) NOT NULL default '',
  `materie` varchar(30) NOT NULL default '',
  `forma` varchar(5) NOT NULL default '',
  `cod` varchar(6) NOT NULL default '',
  `an` tinyint(4) NOT NULL default '0',
  `serie` varchar(5) NOT NULL default '',
  `nr_stud` varchar(6) NOT NULL default '',
  `nr_grupa` tinyint(4) NOT NULL default '0',
  `tip_grupa` varchar(4) NOT NULL default '',
  `modul_c` char(3) NOT NULL default '',
  `modul_a` char(3) NOT NULL default '',
  `tip_curs2` varchar(10) NOT NULL default '',
  `post` tinyint(4) NOT NULL default '0',
  `grad` varchar(5) NOT NULL default '',
  `norma` varchar(30) NOT NULL default '',
  `ocupat` varchar(5) NOT NULL default '',
  `acoperit` varchar(30) NOT NULL default '',
  `acoperit_efect` varchar(30) NOT NULL default '',
  `an_grupa` varchar(12) NOT NULL default '',
  `zi` varchar(10) NOT NULL default '',
  `ora` varchar(8) NOT NULL default '',
  `sala` varchar(10) NOT NULL default '',
  `paritate` tinyint(1) NOT NULL default '1',
  `paritate_start` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`orar_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=101 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `universitate`
-- 

CREATE TABLE `universitate` (
  `univ_id` int(11) NOT NULL auto_increment,
  `nume` varchar(50) NOT NULL default '',
  `data_start` date NOT NULL default '0000-00-00',
  `data_stop` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`univ_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `vacanta`
-- 

CREATE TABLE `vacanta` (
  `vac_id` int(11) NOT NULL auto_increment,
  `link_univ` int(11) NOT NULL default '0',
  `data_start` date NOT NULL default '0000-00-00',
  `data_stop` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`vac_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=9 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `xls_test`
-- 

CREATE TABLE `xls_test` (
  `col1` varchar(20) default NULL,
  `col2` varchar(20) default NULL,
  `col3` varchar(20) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
