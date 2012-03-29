<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript">
<!--
    <?
        if ($_REQUEST['callback']!='') {
            ?>
                var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>();';
                eval(cmd);
            <?
        }
    ?>
    self.close();
//-->
</script>
        <?
    } else {
        ?>
               
<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post" onsubmit="return true;">
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />

<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
    <tr>
        <th>Настройка отображаемых столбцов</th>
    </tr>
    <tr>
    	<td>
    	    <input type="checkbox" name="id" id="id" value="yes" <?=($id === 'yes'?'checked="checked"':'')?> />
    	    <label for="id" class="required" >ID</label>
    	</td>
	</tr>
	<tr>
    	<td>
    	    <input type="checkbox" name="active_flag" id="active_flag" value="yes" <?=($active_flag === 'yes'?'checked="checked"':'')?> />
    	    <label for="active_flag" class="required" >Активный</label>
    	</td>
	</tr>	
      
	<tr>
    	<td>
    	    <input type="checkbox" name="photo" id="photo" value="yes" <?=($photo === 'yes'?'checked="checked"':'')?> />
    	    <label for="photo" class="required" >Фото</label>
    	</td>
	</tr>	
	
	<tr>
    	<td>
    	    <input type="checkbox" name="name" id="name" value="yes" <?=($name === 'yes'?'checked="checked"':'')?> disabled="disabled"/>
    	    <label for="name" class="required" >ФИО</label>
    	</td>
	</tr>	
	
	<tr>
    	<td>
    	    <input type="checkbox" name="department" id="department" value="yes" <?=($department === 'yes'?'checked="checked"':'')?> />
    	    <label for="department" class="required" >Подразделение</label>
    	</td>
	</tr>	
	
	<tr>
    	<td>
    	    <input type="checkbox" name="post" id="post" value="yes" <?=($post === 'yes'?'checked="checked"':'')?> />
    	    <label for="post" class="required" >Должность</label>
    	</td>
	</tr>	

	<tr>
    	<td>
    	    <input type="checkbox" name="email" id="email" value="yes" <?=($email === 'yes'?'checked="checked"':'')?> />
    	    <label for="email" class="required" >E-mail</label>
    	</td>
	</tr>	
	
	<tr>
    	<td>
    	    <input type="checkbox" name="office_phone" id="office_phone" value="yes" <?=($office_phone === 'yes'?'checked="checked"':'')?> />
    	    <label for="office_phone" class="required" >Рабочий телефон</label>
    	</td>
	</tr>

	<tr>
    	<td>
    	    <input type="checkbox" name="room" id="room" value="yes" <?=($room === 'yes'?'checked="checked"':'')?> />
    	    <label for="room" class="required" >Комната</label>
    	</td>
	</tr>
	<tr>
        <td  align="center" ><input type="submit" name="save" value="Сохранить" class="button" /></td>
    </tr>
	

</table>
</form>

        <?
    }
?>