CREATE TABLE IF NOT EXISTS `auth_codes` (
  `code` varchar(40) NOT NULL,
  `client_id` varchar(20) NOT NULL,
  `redirect_uri` varchar(200) NOT NULL,
  `expires` int(11) NOT NULL,
  `scope` varchar(250) DEFAULT NULL,
  `uid` int(10) NOT NULL,
  PRIMARY KEY (`code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- 表的结构 `clients`
--

CREATE TABLE IF NOT EXISTS `clients` (
  `client_id` int(10) NOT NULL AUTO_INCREMENT,
  `client_secret` varchar(32) NOT NULL,
  `redirect_uri` varchar(200) NOT NULL,
  `appname` varchar(30) NOT NULL,
  PRIMARY KEY (`client_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1000000000 ;

-- --------------------------------------------------------

--
-- 表的结构 `tokens`
--

CREATE TABLE IF NOT EXISTS `tokens` (
  `oauth_token` varchar(40) NOT NULL,
  `client_id` varchar(20) NOT NULL,
  `expires` int(11) NOT NULL,
  `scope` varchar(200) DEFAULT NULL,
  `uid` int(10) NOT NULL,
  PRIMARY KEY (`oauth_token`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- 表的结构 `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `uid` int(10) NOT NULL AUTO_INCREMENT,
  `username` varchar(12) NOT NULL,
  `password` varchar(32) NOT NULL,
  `email` varchar(50) NOT NULL,
  `phone` int(11) NOT NULL,
  `img` varchar(200) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

--
-- 转存表中的数据 `user`
--

INSERT INTO `user` (`uid`, `username`, `password`, `email`, `phone`, `img`) VALUES
(1, 'test1', 'test123', 't1@qq.com', 2147483647, 'http://img.mysite.com/test1.jpg'),
(2, 'test2', 'test123', 't2@qq.com', 2147483647, 'http://img.mysite.com/test2.jpg');
