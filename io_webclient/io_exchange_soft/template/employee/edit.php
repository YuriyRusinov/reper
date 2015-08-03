<script language="JavaScript" type="text/javascript">
<!--
function CallbackExecute(close_window)
{
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>();';
    eval(cmd);

    if (close_window) {
        self.close();
    }
}
//-->
</script>
<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript" type="text/javascript">
<!--
    CallbackExecute(true);
//-->
</script>
        <?
    } else {
        ?>
        
        
<? include SITE_FILE_ROOT.'template/ocalendar.php'; ?>
<script language="JavaScript" type="text/javascript">
    function setDate(sDate) {
        document.editform["birthday"].value = sDate;
    }
    function selectDate(event){
        var X=event.clientX+16;
        var Y=event.clientY+16;
        if (X+185>parseInt(document.body.clientWidth)) X=X-185;
        if (Y+215>parseInt(document.body.clientHeight)) Y=Y-215;
        YAHOO.yui.oCalendar.open(document.editform["birthday"].value,"",X,Y,setDate);
    }
   

    function hideCalendar(){
        YAHOO.yui.oCalendar.close();
    }

</script>        
<form name="editform"
      id="editform"
      enctype="multipart/form-data"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>"
      method="post"
      onmousedown="hideCalendar()"
      onsubmit="selectAllLists(document.editform.elements['enabled_employee_roles[]']); return true;">
<input type="hidden" name="id" value="<?= htmlspecialchars($employee->id_employee); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Редактирование данных сотрудника</th>
</tr>
<tr>
    <td align="right">Идентификатор:</td>
    <td width="100%"><?= htmlspecialchars($employee->id_employee); ?></td>
</tr>
<tr>
    <td align="right" class="required">ФИО:</td>
    <td><input type="text" name="name" value="<?= htmlspecialchars($employee->name); ?>" class="wide" /></td>
</tr>

<tr>
    <td align="right">Активный:</td>
    <td>
        <input type="radio" name="active_flag" value="yes" <?= $employee->active_flag=='yes' ? ' checked':'' ?> /> да
        <input type="radio" name="active_flag" value="no" <?= $employee->active_flag!='yes' ? ' checked':'' ?> /> нет
    </td>
</tr>
<tr>
    <td align="right" class="required">Логин:</td>
    <td><input type="text" name="login" value="<?= htmlspecialchars($employee->login); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" class="required">Пароль:</td>
    <td><input type="password" name="password" value="<?= htmlspecialchars($employee->password); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" class="required">E-mail:</td>
    <td><input type="text" name="email" value="<?= htmlspecialchars($employee->email); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" valign="top">Подразделение:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <input type="text" name="department" value="<?= htmlspecialchars($department_name); ?>" class="wide" disabled />
                    <input type="hidden" name="department_id" value="<?= $employee->department_id; ?>" />
                </td>
                <td><button id="select-department" onclick="return SelectDepartmentAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать подразделение" />Выбрать...</button></td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td align="right" valign="top" nowrap>Должность:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="post" class="wide">
                        <option value="" ><!--empty--></option>
                        <?
                            for ($i=0;$i<count($offices);$i++) {
                                ?>
                                    <option value="<?= $offices[$i]->name; ?>"<?= $employee->post==$offices[$i]->name ? ' selected':''; ?>><?= htmlspecialchars($offices[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                <td><button id="select-post" onclick="return SelectPostAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать должность" />Выбрать...</button></td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td align="right" valign="top" nowrap>Вышестоящий руководитель:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="head_id" class="wide">
                        <option value="" ><!--empty--></option>
                        <?
                            for ($i=0;$i<count($employees);$i++) {
                                ?>
                                    <option value="<?= $employees[$i]->id_employee; ?>"<?= $employee->head_id==$employees[$i]->id_employee ? ' selected':''; ?>><?= htmlspecialchars($employees[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                <td><button id="select-head" onclick="return SelectHeadAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать вышестоящего начальника" />Выбрать...</button></td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td align="right" class="top">Фотография:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="100%">
                    <?
                        if ($employee->photo_flag=='yes') {
                            $small_photo = SITE_PHOTO_FILE_ROOT.$employee->id_employee.'_sm.jpg';
                            $size = getimagesize($small_photo);
                            $width = $size[0];
                            $height = $size[1];
                            ?>
                                <img src="<?= SITE_PHOTO_URI_ROOT.$employee->id_employee; ?>_sm.jpg" id="photo" width="<?= $width; ?>" height="<?= $height; ?>" border="0" align="middle" />
                            <?
                        } else {
                            ?>
                                <img src="<?= SITE_URI_ROOT.'img/no_photo.png'; ?>" id="photo" width="<?= PREVIEW_WIDTH; ?>" height="<?= PREVIEW_HEIGHT; ?>" border="0" align="middle" />
                            <?
                        }
                    ?>
                </td>
                <td>
                    <input type="file" name="photo" />
                    <br/>
                    <button id="delete-photo" onclick="return DeletePhoto(<?= $employee->id_employee; ?>);" <?= $employee->photo_flag=='yes' ? '':' disabled'; ?>><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить фотографию" />Удалить</button>
                </td>
            </tr>
        </table>
    </td>
</tr>
<?
if(access_enable($STORAGE,$STORAGE['module'],'role')){
?>
<tr>
    <td colspan="2" valign="top">
        <span class="label required">Принадлежность к ролям:</span><br/>
        <table cellpadding="0" cellspacing="0" border="0" width="100%" class="switchLists">
            <tr>
                <td width="49%">
                    <select name="enabled_employee_roles[]" size="5" class="wide" multiple>
                        <?
                            for ($i=0;$i<count($roles);$i++) {
                                if (!key_exists($roles[$i]->id_role, $enabled_employee_roles)) continue;
                                    ?>
                                        <option value="<?= $roles[$i]->id_role; ?>"><?= htmlspecialchars($roles[$i]->name); ?></option>
                                    <?
                                }
                        ?>
                    </select>
                </td>
                <td valign="middle" align="center" width="1%">
                    <table>
                      <tr>
                        <td><input type="button" value="&gt;" onclick="moveOption(document.editform.elements['enabled_employee_roles[]'], document.editform.elements['disabled_employee_roles[]'])" class="arrowButton" /></td>
                        <td><input type="button" value="&lt;" onclick="moveOption(document.editform.elements['disabled_employee_roles[]'], document.editform.elements['enabled_employee_roles[]'])" class="arrowButton" /></td>
                      </tr>
                      <tr>
                        <td><input type="button" value="&gt;&gt;" onclick="moveAllOptions(document.editform.elements['enabled_employee_roles[]'], document.editform.elements['disabled_employee_roles[]'])" class="arrowButton" /></td>
                        <td><input type="button" value="&lt;&lt;" onclick="moveAllOptions(document.editform.elements['disabled_employee_roles[]'], document.editform.elements['enabled_employee_roles[]'])" class="arrowButton" /></td>
                      </tr>
                    </table>
                </td>
                <td width="49%">
                    <select name="disabled_employee_roles[]" size="5" class="wide" multiple>
                        <?
                            for ($i=0;$i<count($roles);$i++) {
                                if (key_exists($roles[$i]->id_role, $enabled_employee_roles)) continue;
                                    ?>
                                        <option value="<?= $roles[$i]->id_role; ?>"><?= htmlspecialchars($roles[$i]->name); ?></option>
                                    <?
                                }
                        ?>
                    </select>
                </td>
            </tr>
        </table>
    </td>
</tr>
<?
}
?>
<tr>
    <td align="right">Дата рождения (дд.мм.гггг):</td>
    <td>
        <?
            if ($employee->birthday!='') {
                $birthday = format_date('{d}.{m}.{Y}', $employee->birthday);
                if ($birthday=='00.00.0000') $birthday = '';
            }
        ?>
     <input id="year-field" type="text" name="birthday" value="<?= htmlspecialchars($birthday); ?>" class="date"/>
     <button onclick="selectDate(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" id="calbtn"/></button>
    </td>
</tr>
<tr>
    <td align="right">Домашний адрес:</td>
    <td><input type="text" name="home_address" value="<?= htmlspecialchars($employee->home_address); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right">Рабочий телефон:</td>
    <td><input type="text" name="office_phone" value="<?= htmlspecialchars($employee->office_phone); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right">Домашний телефон:</td>
    <td><input type="text" name="home_phone" value="<?= htmlspecialchars($employee->home_phone); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right">Сотовый телефон:</td>
    <td><input type="text" name="cellular" value="<?= htmlspecialchars($employee->cellular); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right">Номер комнаты:</td>
    <td>
        <input type="text" name="room" value="<?= htmlspecialchars($employee->room); ?>" class="date" />
    </td>
</tr>
<tr>
    <td colspan="2" class="line" />
</tr>
<tr class="toolbar">
    <td  align="center" colspan="2"><input type="submit" name="save" value="Сохранить" class="button" /></td>
</tr>
</table>
</form>

<script language="JavaScript" type="text/javascript">
<!--
function SetDepartmentAction(value, text)
{
    document.editform.elements['department'].value = text;
    document.editform.elements['department_id'].value = value;
}

function SetHeadAction(value, text)
{
    var list = document.editform.elements['head_id'];

    selectOption(list, value);
}

function SelectDepartmentAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>department/select-department/callback/SetDepartmentAction';

    var w = 500;
    var h = 400;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SelectHeadAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>employee/select-employee/callback/SetHeadAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SelectPostAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>office/select-office/callback/SetPostAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SetPostAction(value, text)
{
    var list = document.editform.elements['post'];

    selectOption(list, value);
}





function DeletePhotoAction()
{
    var img = document.getElementById('photo');
    img.width = <?= PREVIEW_WIDTH; ?>;
    img.height= <?= PREVIEW_HEIGHT; ?>;
    img.src = '<?= SITE_HTTP_ROOT.'img/no_photo.png'; ?>';

    CallbackExecute(false);
}


function DeletePhoto(id)
{
    if (confirm('Вы действительно хотите удалить фотографию?')) {
        var url = '<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/delete-photo/id/' + id + '/callback/DeletePhotoAction';

        var w = 1;
        var h = 1;
        var param = '';
        OpenPopupWindow(url, w, h, param);
    }

    return false;
}

//-->
</script>

        <?
    }
?>