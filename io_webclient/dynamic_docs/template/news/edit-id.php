<? include SITE_FILE_ROOT.'template/ocalendar.php'; ?>
<script language="JavaScript" type="text/javascript">
    function setDate(sDate) {
        document.editform["day"].value = sDate;
    }
    function selectDate(event){
        var X=event.clientX+16;
        var Y=event.clientY+16;
        if (X+185>parseInt(document.body.clientWidth)) X=X-185;
        if (Y+215>parseInt(document.body.clientHeight)) Y=Y-215;
        YAHOO.yui.oCalendar.open(document.editform["day"].value,"",X,Y,setDate);
    }
</script>

<div class="pagename">
    <?=$title?>
</div>
<form name="editform"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" 
      method="post" 
      enctype="multipart/form-data" >
<input type="hidden" name="id" value="<?=$news->news_id;?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="formz" align="center">
<tr>
<td width="10%">
<table cellpadding="5" cellspacing="0" border="0" width="750" >
<tr>
    <td valign=top width="15%">Дата (дд.мм.гггг):</td>
    <td><input type="text" name="day" value="<?=format_date('{d}.{m}.{Y}',$news->news_date);?>" class="date" />
        <button onclick="selectDate(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" /></button>
    </td>
</tr>
<tr>
    <td valign=top>Картинка:</td>
    <td>
        <input type="radio" name="img" value="l" id="imgl" <?=($news->news_img=='l'?'checked="checked"':'')?> onclick="showFile(true)" /> <label for="imgl">Картинка слева</label><br/>
        <input type="radio" name="img" value="n" id="imgn" <?=($news->news_img=='n'?'checked="checked"':'')?> onclick="showFile(false)"/> <label for="imgn">Нет картинки</label><br/>
        <input type="radio" name="img" value="r" id="imgr" <?=($news->news_img=='r'?'checked="checked"':'')?> onclick="showFile(true)" /> <label for="imgr">Картинка справа</label><br/>
        <div id="picturefile" <?=($news->news_img=='n'?'style="visibility:hidden"':'')?>>
        Картинка: <input type="file" class="button" name="file" /> <br/> 
        Допустимые расширения *.GIF, *.JPG, *.PNG
        </div><br/>
    </td>
</tr>
<tr>
    <td>Заголовок:</td>
    <td><input type=text name="title" style="width: 100%;" value="<?=$news->news_title?>" /></td>
</tr>
<tr>
    <td>&nbsp;</td>
    <td>
        <a href="javascript:AddTags('<b>','</b>')" title="полужирный" class=buten><img src="<?= SITE_URI_ROOT; ?>img/bold.gif" width="14" height="14" border=0 alt="полужирный" /></a>
        <a href="javascript:AddTags('<i>','</i>')" title="курсив" class=buten><img src="<?= SITE_URI_ROOT; ?>img/italic.gif" width="14" height="14" border=0 alt="курсив" /></a>
        <a href="javascript:AddTags('<u>','</u>')" title="подчеркнутый" class=buten><img src="<?= SITE_URI_ROOT; ?>img/underline.gif" width="14" height="14" border=0 alt="подчеркнутый" /></a>
        <a href="javascript:AddTags('<center>','</center>')" title="выравнивание по центру" class=buten><img src="<?= SITE_URI_ROOT; ?>img/center.gif" width="45" height="14" border=0 alt="выравнивание по центру" /></a>
        <a href="javascript:AddTags('&laquo;','&raquo;')" title="фигурные кавычки" class=buten><img src="<?= SITE_URI_ROOT; ?>img/figa.gif" width="20" height="14" border=0 alt="фигурные кавычки" /></a>
        <a href="javascript:AddTags('&quot;','&quot;')" title="простые кавычки" class=buten><img src="<?= SITE_URI_ROOT; ?>img/k.gif" width="20" height="14" border=0 alt="простые кавычки" /></a>
        <a href="javascript:AddTags('<br>','')" title="перенос строки" class=buten><img src="<?= SITE_URI_ROOT; ?>img/br.gif" width="20" height="14" border=0 alt="перенос строки" /></a>
        <a href="javascript:AddTags('\n<p>','\n ')" title="абзац" class=buten><img src="<?= SITE_URI_ROOT; ?>img/p.gif" width="20" height="14" border=0 alt="абзац" /></a>
        <a href="javascript:AddTags('<a href=>','</a>')" title="ссылка" class=buten><img src="<?= SITE_URI_ROOT; ?>img/anchor.gif" width="14" height="14" border=0 alt="ссылка" /></a>
    </td>
<tr>
    <td valign=top>Содержание:</td>
    <td><textarea name="content" style="height: 250px; width: 100%;"><?=$news->news_content?></textarea>
    </td>
</tr>
<tr class="toolbar">
    <td valign=top></td>
    <td><button type="reset" class="button" >Очистить</button>
        <button class="button" onclick="showPreview();return false;">Предпросмотр текста</button>
        <button type=submit class="button" >Сохранить</button>
    </td>
</tr>
</table>
</td>
<td>
&nbsp;
</td>
<tr>
</table>
</form>

<script type="text/javascript" language="JavaScript">
function AddTags(Tag,Tag2)
{
    document.editform.content.focus();
    sel = document.selection.createRange();
    sel.text = Tag+sel.text+Tag2;
    document.editform.content.focus();
}
function showFile(show)
{
    if(show){
        document.getElementById("picturefile").style.visibility="visible";
    }else{
        document.getElementById("picturefile").style.visibility="hidden";
    }
}
function showPreview()
{
    window.open('<?=SITE_HTTP_ROOT.$STORAGE['module']?>/preview',this.target,'width=600,height=350,'+'location=no,toolbar=no,menubar=no,status=yes,resizeable=yes,scrollbars=yes');
    return false;
}
</script>
