<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title><?= isset($page_title) ? $page_title : SITE_NAME; ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Content-Language" content="ru" />
<meta http-equiv="imagetoolbar" content="no" />
<meta name="Description" content="<?= isset($page_description) ? $page_description:''; ?>" />
<meta name="Keywords" content="<?= isset($page_keywords) ? $page_keywords:''; ?>" />
<meta name="Robots" content="index, follow" />

<link type="text/css" rel="stylesheet" href="<?= SITE_URI_ROOT; ?>js/yui/calendar/assets/skins/sam/calendar.css" />
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/yahoo/yahoo.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/event/event.js" ></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/dom/dom.js" ></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/yahoo-dom-event/yahoo-dom-event.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/container/container_core.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/yui/calendar/calendar.js"></script>

<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/common.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/movesel.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/prototype.js"></script>
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
//-->
</script>
<link rel="stylesheet" type="text/css" href="<?= SITE_URI_ROOT; ?>css/style.css" />
</head>
<body>
<div id="JSErrorContainer" class="hidden"></div>
<div id="Content">
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