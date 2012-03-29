<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Данные сотрудника</th>
</tr>
<tr>
    <td align="right">Идентификатор:</td>
    <td width="100%"><?= htmlspecialchars($employee->id_employee); ?></td>
</tr>
<tr>
    <td align="right" class="required">ФИО:</td>
    <td><?= htmlspecialchars($employee->name); ?></td>
</tr>
<tr>
    <td align="right" class="required">E-mail:</td>
    <td><a href="mailto:<?= $employee->email; ?>"><?= htmlspecialchars($employee->email); ?></a></td>
</tr>
<tr>
    <td align="right" valign="top">Подразделение:</td>
    <td><?= htmlspecialchars($department_name); ?></td>
</tr>
<tr>
    <td align="right" valign="top" nowrap>Вышестоящий руководитель:</td>
    <td><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>id/<?= $head->id_employee; ?>/"><?= htmlspecialchars($head->name); ?></a></td>
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
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td align="right">Должность:</td>
    <td><?= htmlspecialchars($employee->post); ?></td>
</tr>
<tr>
    <td align="right">Дата рождения:</td>
    <td>
        <?
            if ($employee->birthday!='') {
                $birthday = format_date('{d}.{m}.{Y}', $employee->birthday);
                if ($birthday=='00.00.0000') $birthday = '';
            }
        ?>
        <?= htmlspecialchars($birthday); ?>
    </td>
</tr>
<tr>
    <td align="right">Домашний адрес:</td>
    <td><?= htmlspecialchars($employee->home_address); ?></td>
</tr>
<tr>
    <td align="right">Рабочий телефон:</td>
    <td><?= htmlspecialchars($employee->office_phone); ?></td>
</tr>
<tr>
    <td align="right">Домашний телефон:</td>
    <td><?= htmlspecialchars($employee->home_phone); ?></td>
</tr>
<tr>
    <td align="right">Сотовый телефон:</td>
    <td><?= htmlspecialchars($employee->cellular); ?></td>
</tr>
<tr>
    <td align="right">Номер комнаты:</td>
    <td><?= htmlspecialchars($employee->room); ?></td>
</tr>
</table>
