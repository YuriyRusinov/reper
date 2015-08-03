<?
$pref = "_1";

$st_url = SITE_NEWS_HTTP_ROOT;
$ul = SITE_HTTP_ROOT.$STORAGE['module']."/".$STORAGE['action']; 

if (count($newses)>0){
	$news= new news();
?>
    <br/>
    <table cellspacing="0" cellpadding="2" width="<?=$tab_width?>" border="0" class="list">
    <tbody>
<?
    for($i=0;$i < count($newses);$i++)
    {
        $news->Init();
        $news->LoadItem($newses[$i]);
        
        $title=$news->news_title;
        if ($p_date == 1) {$title = format_date('{d}.{m}.{Y}', $news->news_date).'&nbsp;'.$title;} 
        if ($p_time == 1) {$title = $news->news_time.'&nbsp;'.$title;}
        //положение картинки
        //l слева
        //r справа
        //n ее нет
        if ($news->news_img == 'l'){
            $image='<img src="'.SITE_NEWS_HTTP_ROOT.$news->news_stamp.$pref.$news->news_exp.'" align="left" style="padding:0 10px 0 0;"  border="0" height="'.$img_height.'"/>';
        }elseif ($news->news_img  == 'n'){
            $image='';    
        }elseif ($news->news_img == 'r'){
            $image='<img src="'.SITE_NEWS_HTTP_ROOT.$news->news_stamp.$pref.$news->news_exp.'" align="left" style="padding:0 10px 0 0;" border="0" height="'.$img_height.'"/>';
        }
        
        $LengthC=@strpos($news->news_content, ' ', $PreLengthC);
        if(!$LengthC){
            $LengthC=strlen($news->news_content);
        }
        ?>
            <tr><th bgcolor="#E6E6E6"><?=$title?></th></tr>
            <tr><td width="100%" valign="top" >
                <?=$image.substr($news->news_content, 0, $LengthC);?> ... 
                <a href="<?=SITE_HTTP_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/id/'.$news->news_id.'/';?>">Просмотреть полностью&nbsp;&gt;&gt;&gt;</a>
                </td>
            </tr>
            <tr><td>&nbsp;</td></tr>
        <?
    }
?>
    </tbody>
    </table>
    <br/>
<?
}else {
?>    
    Нет новостей!
<?    
};

?>
