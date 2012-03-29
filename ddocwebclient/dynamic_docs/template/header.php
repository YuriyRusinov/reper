<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title><?= SITE_NAME; ?><?= isset($page_title) ? ' - '.$page_title :''; ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Content-Language" content="ru" />
<meta http-equiv="imagetoolbar" content="no" />
<meta name="Description" content="<?= isset($page_description) ? $page_description:''; ?>" />
<meta name="Keywords" content="<?= isset($page_keywords) ? $page_keywords:''; ?>" />
<meta name="Robots" content="index, follow" />

<link rel="shortcut icon" href="<?= SITE_URI_ROOT; ?>favicon.ico" />
<link type="text/css" rel="stylesheet" href="<?= SITE_URI_ROOT; ?>js/yui/calendar/assets/skins/sam/calendar.css" />
<link type="text/css" rel="stylesheet" href="<?= SITE_URI_ROOT; ?>js/yui/calendar/assets/skins/eco/calendar-skin.css" />
<link type="text/css" rel="stylesheet" href="<?= SITE_URI_ROOT; ?>js/yui/tabview/assets/skins/sam/tabview.css" /> 
<link type="text/css" rel="stylesheet" href="<?= SITE_URI_ROOT; ?>js/yui/tabview/assets/skins/eco/tabview-skin.css" /> 
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/yahoo/yahoo.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/event/event.js" ></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/dom/dom.js" ></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/yahoo-dom-event/yahoo-dom-event.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/element/element-beta-min.js"></script> 
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/container/container_core.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/calendar/calendar.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/tabview/tabview-min.js"></script> 
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/json/json-min.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/datasource/datasource-min.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/charts/charts-experimental-min.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/animation/animation-min.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/autocomplete/autocomplete-min.js"></script>




<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/common.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/movesel.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/prototype.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/lightbox.js"></script>


<script type="text/JavaScript">
<!--
function ErrHandler(msg, url, lno)
{
    var element_name = 'JSErrorContainer';
    var element = $(element_name);

    if (element) {
        ShowElement(element);
        element.update('<font color="red"><b>Ошибки JavaScript (этап отладки):<\/b> ' + msg + ' [' + url + ':' + lno + ']<\/font>');
    }

    return true;
}
//window.onerror = ErrHandler;

function Ping()
{
    var str = "LoadPage('<?= SITE_HTTP_ROOT ?>online-ping.php');";
    eval(str);
    setTimeout('Ping();', <?= PING_TIME_DIFF_SEC*1000; ?>);
}
window.onload = Ping;

//-->
</script>
<link rel="stylesheet" type="text/css" href="<?= SITE_URI_ROOT; ?>css/style.css" />
<!--[if lt IE 7]>
<link rel="stylesheet" type="text/css" href="<?= SITE_URI_ROOT; ?>css/style6.css" />
<![endif]-->

</head>
<body>
<?
    if (!(defined('DISABLE_MENU') && DISABLE_MENU)) {
        $current_employee = $_SESSION['current_employee'];
        $months_long=array("января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря");
        $weekdays_long=array("Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота");
        

?>
    &nbsp;
    <table id="headerTable" cellspacing="0" cellpadding="0" border="0" >
        <tr>
           <td width="200" align="left" valign="top">
               <img id="logo" src="<?=SITE_URI_ROOT?>img/logo.gif" width="35" height="35" />
           </td>
           <td width="130" align="left" valign="top">
               <?=date("j").' '.$months_long[date("n")-1].' '.date("Y");?>
               <br/>
               <?=$weekdays_long[date("w")];?>               
           </td>           
           <th width="90" align="left" valign="top">
               Пользователь: 
           </th>    
           <td width="250" align="left" valign="top">
              <?= htmlspecialchars($_SESSION['DB_USER']); ?>
           </td>           
           <td width="350" align="left" valign="top">
              <table cellspacing="0" cellpadding="0" border="0">
                  <tr>
                      <th>Должность:&nbsp;</th><td><?=$_SESSION['PST_NAME'];?></td>
                  </tr>    
              </table>
           </td>
           <td width="350" align="left" valign="top">
              <table cellspacing="0" cellpadding="0" border="0">
                  <tr>
                      <th>Отдел:&nbsp;</th><td><?=$_SESSION['DEP_NAME'];?></td>
                  </tr>
                  
              </table>
           </td>
            <td width="350" align="left" valign="top">
              <table cellspacing="0" cellpadding="0" border="0">
                  <tr>
                      <th>Режим работы:&nbsp;</th><td><?=$_SESSION['USER_LEVEL'];?></td>
                  </tr>    
              </table>
           </td>
           
             <td width="100" align="left" valign="top">
              <table cellspacing="0" cellpadding="0" border="0">
                  <tr>
                      <th>&nbsp;</th><td> 
                      <?
                      $img = "<img src='".SITE_URI_ROOT."data/".session_id().".jpg'>";
                       echo $img;
                      ?> 
                      
                      </td>
                  </tr>    
              </table>
           </td>
           
                          
        </tr>
    </table>
<?
        $current_employee = $_SESSION['current_employee'];
?>
        <script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/menu.js"></script>
        <!-- menu //-->
        <div>
                <div id="menu" >
                    <?= build_menu($STORAGE['menu']); ?>
                    <div id="logoutMenuMessage"><b><a href="<?= SITE_URI_ROOT; ?>logout.php">Выйти</a></b></div>
                </div>
                <div id="submenu" >
                    &nbsp;
                </div>
        </div>
        <!-- /menu //-->
    <table cellspacing="0" cellpadding="0" border="0" id="mainTable">
    <tr>
    <td id="leftColumn">
        <!-- site path //-->
        <div id="sitePath">
                <ul>
                    <li class="first"><a href="<?= SITE_URI_ROOT; ?>">Главная</a></li>
                    <?
                        for ($i=0;$i<count($STORAGE['site_path']);$i++) {
                            ?>
                                <li><a href="<?= SITE_URI_ROOT.$STORAGE['site_path'][$i]['path']; ?>"<?= array_key_exists('popup_open', $STORAGE['site_path'][$i]) && $STORAGE['site_path'][$i]['popup_open'] ? ' onclick="OpenPopupWindow(\''.SITE_HTTP_ROOT.$STORAGE['site_path'][$i]['path'].'\', '.$STORAGE['site_path'][$i]['width'].', '.$STORAGE['site_path'][$i]['height'].')"':''; ?>><?= $STORAGE['site_path'][$i]['name']; ?></a></li>
                            <?
                        }
                    ?>
                </ul>
        </div>
        <!-- site path //-->
<?
    }
?>
<div id="JSErrorContainer" class="hidden"></div>
<div id="content">

<?php
    if (isset($errors) && count($errors)>0) {
        ?>
            <div id="errorBlock">
            <center>
                <div class="errors"><b>Ошибки:</b><br />
                    <?php
                        for ($i=0;$i<count($errors);$i++) {
                            echo ($i+1).'. '.$errors[$i].'<br />';
                        }
                    ?>
                </div>
                <br />
             </center>
             </div>
        <?php
    }
?>
