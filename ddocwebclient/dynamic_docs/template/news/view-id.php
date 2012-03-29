<?
if(access_enable($STORAGE, $STORAGE['module'], 'edit')){  
?>
<div id="news-toolbar" class="toolbar">
<button id="news-0" onclick="EditAction()"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Редактировать" />Редактировать</button>
</div>
<script type="text/javascript" language="JavaScript">
function EditAction()
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/<?=$news->news_id?>/';
                window.location = url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}
</script>
<?
}

$title=$news->news_title;
if ($p_date == 1){$title = format_date('{d}.{m}.{Y}', $news->news_date).'&nbsp;'.$title;} 
if ($p_time == 1) {$title = $news->news_time.'&nbsp;'.$title;}
//положение картинки
//l слева
//r справа
//n ее нет
if ($news->news_img == 'l'){
    $image='<img src="'.SITE_NEWS_HTTP_ROOT.$news->news_stamp.$news->news_exp.'" align="left" hspace="10" vspace="5" border="0" />';
}elseif ($news->news_img  == 'n'){
    $image='';    
}elseif ($news->news_img == 'r'){
    $image='<img src="'.SITE_NEWS_HTTP_ROOT.$news->news_stamp.$news->news_exp.'" align="right" hspace="10" vspace="5" border="0" />';
}
?>
<br/>
<table cellspacing="0" cellpadding="4" width="<?=$tab_width?>" border="0" class="list">
<tbody>
    <tr><th bgcolor="#E6E6E6"><?=$title?></th></tr>
    <tr><td width="100%" valign="top" bgcolor=#F4F4F4><?=$image.$news->news_content;?></td></tr>
</tbody>
</table>
