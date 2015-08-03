<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>("<?= htmlspecialchars($type); ?>", <?= intval($id); ?>);';
    eval(cmd);
    self.close();
//-->
</script>
        <?
    } else {
        ?>
<form name="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post" onsubmit="selectAllLists(document.editform.elements('attach_files[]'), document.editform.elements('disable_attach_files[]')); return true;">
<input type="hidden" name="id" value="<?= htmlspecialchars($id); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Присоединение файлов</th>
</tr>
<tr>
    <td colspan="2" valign="top">
        <span class="label required">Файлы:</span><br>
        <table cellpadding="0" cellspacing="0" border="0" width="100%" class="switchLists">
            <tr>
                <td width="49%">
                    <select name="attach_files[]" size="5" class="wide" multiple>
                        <?
                            for ($i=0;$i<count($files);$i++) {
                                if (!key_exists($files[$i]['id_file'], $attach_files)) continue;
                                    ?>
                                        <option value="<?= $files[$i]['id_file']; ?>"><?= htmlspecialchars($files[$i]['name']); ?></option>
                                    <?
                                }
                        ?>
                    </select>
                </td>
                <td valign="middle" align="center" width="1%">&nbsp;<input type="button" value="&gt;" onclick="moveOption(document.editform.elements['attach_files[]'], document.editform.elements['disable_attach_files[]'])" class="arrowButton">&nbsp;<input type="button" value="&lt;" onclick="moveOption(document.editform.elements['disable_attach_files[]'], document.editform.elements['attach_files[]'])" class="arrowButton">&nbsp;<br><br>&nbsp;<input type="button" value="&gt;&gt;" onclick="moveAllOptions(document.editform.elements['attach_files[]'], document.editform.elements['disable_attach_files[]'])" class="arrowButton">&nbsp;<input type="button" value="&lt;&lt;" onclick="moveAllOptions(document.editform.elements['disable_attach_files[]'], document.editform.elements['attach_files[]'])" class="arrowButton">&nbsp;</td>
                <td width="49%">
                    <select name="disable_attach_files[]" size="5" class="wide" multiple>
                        <?
                            for ($i=0;$i<count($files);$i++) {
                                if (key_exists($files[$i]['id_file'], $attach_files)) continue;
                                    ?>
                                        <option value="<?= $files[$i]['id_file']; ?>"><?= htmlspecialchars($files[$i]['name']); ?></option>
                                    <?
                                }
                        ?>
                    </select>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td colspan="2" class="line" />
</tr>
<tr>
    <td  align="center" colspan="2"><input type="submit" name="save" value="Сохранить" class="button" /></td>
</tr>
</table>
</form>
        <?
    }
?>