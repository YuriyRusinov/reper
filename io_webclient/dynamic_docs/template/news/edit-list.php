
<table cellpadding=5 cellspacing=2 border=0 width=100%>
<tr>
<td valign=top>
<?
$year = format_date('{Y}', $startdate);
$month = format_date('{m}', $startdate);

if ($today > $numdays) { $today--; }
if($month == "01" ){$month_ru="Январь";}
elseif($month == "02" ){$month_ru="Февраль";}
elseif($month == "03" ){$month_ru="Март";}
elseif($month == "04" ){$month_ru="Апрель";}
elseif($month == "05" ){$month_ru="Май";}
elseif($month == "06" ){$month_ru="Июнь";}
elseif($month == "07" ){$month_ru="Июль";}
elseif($month == "08" ){$month_ru="Август";}
elseif($month == "09" ){$month_ru="Сентябрь";}
elseif($month == "10"){$month_ru="Октябрь";}
elseif($month == "11"){$month_ru="Ноябрь";}
elseif($month == "12"){$month_ru="Декабрь";}

$prev_month=date('Y-m-d',strtotime('-1 month', strtotime($startdate)));
$next_month=date('Y-m-d',strtotime('+1 month', strtotime($startdate)));
$prev_year=date('Y-m-d',strtotime('-1 year', strtotime($startdate)));
$next_year=date('Y-m-d',strtotime('+1 year', strtotime($startdate)));
?>
    <table cellpadding="3" cellspacing="1" border="0" class="list" >
        <tr>
            <th><a href="<?=SITE_HTTP_ROOT.$STORAGE['module']?>/edit/date/<?=$prev_year;?>/ " title="Год назад"><img src="<?= SITE_URI_ROOT; ?>img/arrow_l.gif" width=17 height=17 border=0 /></a></th> 
            <th><a href="<?=SITE_HTTP_ROOT.$STORAGE['module']?>/edit/date/<?=$prev_month;?>/ " title="Месяц назад"><img src="<?= SITE_URI_ROOT; ?>img/arrow_l.gif" width=17 height=17 border=0 /></a></th>
            <th><a href="<?=SITE_HTTP_ROOT.$STORAGE['module']?>/edit/date/<?=$next_month;?>/ " title="Месяц вперед"><img src="<?= SITE_URI_ROOT; ?>img/arrow_r.gif" width=17 height=17 border=0 /></a></th>
            <th><a href="<?=SITE_HTTP_ROOT.$STORAGE['module']?>/edit/date/<?=$next_year;?>/ " title="Год вперед"><img src="<?= SITE_URI_ROOT; ?>img/arrow_r.gif" width=17 height=17 border=0 /></a></th>
        </tr>
        <tr class="odd">
            <td colspan="4"><div class="center"><b><?=$month_ru.'&nbsp;'.$year;?></b></div></td>
        </tr>
    <?
    for($i=0;$i<count($news_dates);$i++){
    ?>    
        <tr>
            <td colspan="4">
            <div class="center">
            <a href="<?=SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/date/'.$news_dates[$i]['news_date'].'/'?>">
                <?=format_date('{d}.{m}.{Y}', $news_dates[$i]['news_date']);?>
            </a>
            </div>
            </td>
       </tr>
    <?    
    }
    ?>
    </table>
</td>

<td width=100% valign=top>
<?
if(count($newses) == 0)
{
    echo '<center><b><font color=red>Нет объявлений за '.format_date('{d}.{m}.{Y}',$linedate).' !</font></b></center>';
}
?>
    <table cellpadding="3" cellspacing="1" border="0" width="100%" class="list">
    <tr>
        <th>№</th>
        <th>Время</th>
        <th>Дата</th>
        <th>Заголовок</th>
        <th>Содержание</th>
        <th>Операции</th>
    </tr>
<?
$news =new news();
$odd=false;
for($i=0;$i < count($newses);$i++)
{
    $odd=!$odd;
    $news->Init();
    $news->LoadItem($newses[$i]);
    
    ?>
    <tr <?=($odd?'class="odd"':'');?>>
        <td> <?=$i+1?>. </td>
	    <td> <?=$news->news_time; ?></td>
	    <td width="12%"><?=format_date('{d}.{m}.{Y}',$news->news_date);?></td>
        <td width="38%"><?=$news->news_title?></td>
        <td width="40%"><?=$news->news_content?></td>
        <td nowrap valign="top">
        <?if($is_enabled_news_edit){?>
            <button id="edit-news-<?= $news->news_id ?>" onclick="EditAction(<?= $news->news_id ?>)" title="Редактировать объявление"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
        <?}?>
        <?if($is_enabled_news_delete){?>    
            <button id="delete-news-<?= $news->news_id ?>" onclick="DeleteAction(<?= $news->news_id ?>)" title="Удалить"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" /></button>
        <?}?>
        </td>
    </tr>
<?
}
?>
    </table>
</td>
</tr>
</table>
<script type="text/javascript" language="JavaScript">
function EditAction(id)
{
    <?
        if ($is_enabled_news_edit) {
            ?>
                       
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/' + id + '/';
                window.location = url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}


function DeleteAction(id)
{
    <?
        if ($is_enabled_news_delete) {
            ?>
                if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
                   
                  
                    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/delete/id/' + id + '/';
                    window.location = url;
                }
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

</script>

