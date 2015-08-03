<?= $page_break; ?>
<div id="employee-toolbar" class="toolbar left">
<button id="edit-role-0" <?=($is_enabled_employee_edit?'onclick="EditAction(0)"':'disabled="disabled"')?>><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить сотрудника" />Добавить</button>
<button id="edit-rows-0" <?=($is_enabled_rows_conf?'onclick="EditRows(0)"':'disabled="disabled"')?>><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать столбцы" />Настройка столбцов</button>
</div>
<table cellpadding="3" cellspacing="1" border="0" class="list">
    <tr>
        <th>№</th>
    <?
    $colspan=2;
    if($id === 'yes') {
    	$colspan++;
     	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/id_employee/sort_dir/<?= $sort=='id_employee' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">ID</a><?= $sort=='id_employee' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($active_flag === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/active_flag/sort_dir/<?= $sort=='active_flag' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">Акт</a><?= $sort=='active_flag' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($photo === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/photo_flag/sort_dir/<?= $sort=='photo_flag' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">Фото</a><?= $sort=='photo_flag' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($name === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/name/sort_dir/<?= $sort=='name' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">ФИО</a><?= $sort=='name' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($department === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/department/sort_dir/<?= $sort=='department' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">Подразделение</a><?= $sort=='department' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($post === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/post/sort_dir/<?= $sort=='post' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">Должность</a><?= $sort=='post' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($email === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/email/sort_dir/<?= $sort=='email' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">E-mail</a><?= $sort=='email' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($office_phone === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/office_phone/sort_dir/<?= $sort=='office_phone' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">Раб.тел</a><?= $sort=='office_phone' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    if($room === 'yes') {
    	$colspan++;
      	?>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/department/<?=$department_id?>/from/0/sort/room/sort_dir/<?= $sort=='room' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>/" onclick="return Aclick(this)">Комната</a><?= $sort=='room' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <?
    }
    ?>
        <th>Операции</th>
    </tr>
    <?
    $odd=false;
    $prev_dep_id=-1;
 	$employee = new employee();
    for ($i=0; $i<count($employees); $i++) {
        $odd=!$odd;
        $employee->Init();
    	$employee->LoadItem($employees[$i]);
    	if (($sort=='department') && ($prev_dep_id!=$employee->department_id)) {
    	    $odd=true;
            ?>
            	<tr>
            	    <th align="left" colspan="<?=$colspan?>">
            	    <? 
            	    $dl='';
            	    for ($n=count($employees[$i]['department'])-1; $n>0; $n--) {
            	    	$dl.=htmlspecialchars($employees[$i]['department'][$n]['dep']).' :: ';
            	    }
            	    $dl.=htmlspecialchars($employees[$i]['department'][0]['dep']);
            	    ?>
            	      <?=$dl?>&nbsp;
            	    </th>
            	</tr>
            <?	
            $prev_dep_id=$employee->department_id;
    	}
            ?>
                <tr <?=($odd?'class="odd"':'')?>>
                    <td class="center top"><?= $i+$from+1 ?></td>
                <?
                if($id === 'yes') {
     	          ?>
                    <td class="center top"><?= $employee->id_employee; ?></td>
                  <?
                }
                if($active_flag === 'yes') {
      	          ?>
                    <td class="center top"><?= $employee->active_flag=='yes' ? 'Да':'Нет'; ?></td>
                  <?
                }
                if($photo === 'yes') {
      	          ?>
                    <td class="center">
                        <?
                        if ($employee->photo_flag=='yes') {
                        	$small_photo = SITE_PHOTO_FILE_ROOT.$employee->id_employee.'_sm.jpg';
                        	$size = getimagesize($small_photo);
                        	$width = $size[0];
                        	$height = $size[1];
                                ?>
                                    <img src="<?= SITE_PHOTO_URI_ROOT.$employee->id_employee; ?>_sm.jpg" width="<?= $width; ?>" height="<?= $height; ?>" border="0" />
                                <?
                        } else {
                                ?>
                                    <img src="<?= SITE_URI_ROOT.'img/no_photo.png'; ?>" width="<?= PREVIEW_WIDTH; ?>" height="<?= PREVIEW_HEIGHT; ?>" border="0" />
                                <?
                        }
                        ?>
                    </td>
                  <?
                }
                if($name === 'yes') {
      	          ?>
                    <td class="top" <?='title="Вышестоящий руководитель'.($employees[$i]['head']==''?' не указан':': '.htmlspecialchars($employees[$i]['head'])).'"'?>><?= htmlspecialchars($employee->name); ?>&nbsp;</td>
                  <?
                }
                if($department === 'yes') {
      	          ?>
                    <td class="top" >
            	    <? 
            	    $dl='';
            	    for ($n=count($employees[$i]['department'])-1; $n>0; $n--) {
            	    	if ($employees[$i]['department'][$n]['head']!=''){
            	    		$dl.='<i title="Начальник: '.htmlspecialchars($employees[$i]['department'][$n]['head']).'">'.htmlspecialchars($employees[$i]['department'][$n]['dep']).'</i>::<br/>';
            	    	}
            	    	else
            	    	{
            	    		$dl.='<i title="Начальник не указан">'.htmlspecialchars($employees[$i]['department'][$n]['dep']).'</i>::<br/>';
            	    	}
            	    };
            	    if ($employees[$i]['department'][0]['head']!=''){
            	    	$dl.='<span title="Начальник: '.htmlspecialchars($employees[$i]['department'][0]['head']).'">'.htmlspecialchars($employees[$i]['department'][0]['dep']).'</span>';
            	    }
            	    else
            	    {
            	    	$dl.='<span title="Начальник не указан">'.htmlspecialchars($employees[$i]['department'][0]['dep']).'</span>';
            	    };
            	    ?>
            	      <?=$dl?>&nbsp;
                    </td>
                  <?
                }
                if($post === 'yes') {
      	          ?>
                    <td class="top" ><?= htmlspecialchars($employee->post); ?>&nbsp;</td>
                  <?
                }
                if($email === 'yes') {
      	          ?>
                    <td class="top"><a href="mailto:<?= htmlspecialchars($employee->email); ?>"><?= htmlspecialchars($employee->email); ?></a>&nbsp;</td>
                  <?
                }
                if($office_phone === 'yes') {
      	          ?>
                    <td class="top"><?= htmlspecialchars($employee->office_phone); ?>&nbsp;</td>
                  <?
                }
                if($room === 'yes') {
      	          ?>
                    <td class="top"><?= htmlspecialchars($employee->room); ?>&nbsp;</td>
                  <?
                }
	        	?>
                    <td class="top">

                        <button id="view-employee-<?= $employee->id_employee; ?>" onclick="DetailAction(<?= $employee->id_employee; ?>)" title="Просмотреть данные сотрудника"><img src="<?= SITE_URI_ROOT; ?>img/iconDetail.gif" /></button>

<?
                    if($is_enabled_employee_edit){
?>
                        <button id="edit-employee-<?= $employee->id_employee; ?>" onclick="EditAction(<?= $employee->id_employee; ?>)" title="Редактировать данные сотрудника"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
<?
                    };
?>
<?
                    if($is_enabled_employee_delete){
?>
                        <button id="delete-employee-<?= $employee->id_employee; ?>" onclick="DeleteAction(<?= $employee->id_employee; ?>)" title="Удалить сотрудника" ><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" /></button>
<?
                    };
?>
                    </td>
                </tr>
            <?
    }
    ?>
</table>

<?
if (!$ajax_request) {
?>	

<script type="text/javascript" language="JavaScript">
<!--
    function DetailAction(id){
		var w = 550;
		var h = 200;
		var param = '';
		var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/detail/id/' + id + '/';
		OpenPopupWindow(url, w, h, param);
    }

<?
if($is_enabled_employee_edit){
	?>
	function EditAction(id)
	{
		var w = 550;
		var h = 200;
		var param = '';
		var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/' + id + '/callback/Refresh/';
		OpenPopupWindow(url, w, h, param);
	}
	<?
}

if($is_enabled_employee_delete){
	?>
	function DeleteAction(id)
	{
		if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
			var w = 1;
			var h = 1;
			var param = '';
			var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/delete/id/' + id + '/callback/Refresh/';
			OpenPopupWindow(url, w, h, param);
		}
	}
	<?
}

if($is_enabled_rows_conf){
	?>
	function EditRows(id )
	{
		var w = 550;
		var h = 200;
		var param = '';
		var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/rows_conf/id/' + id + '/callback/Refresh/';
		OpenPopupWindow(url, w, h, param);
	}
	<?
}
?>

function Refresh()
{
	var offsetX = document.body.scrollLeft;
	var offsetY = document.body.scrollTop;
	var location = "<?= SITE_HTTP_ROOT.preg_replace('/\/x\/\d+\/y\/\d+/', '', $STORAGE['request_uri']); ?>";

	window.location = location + 'x/' + offsetX + '/y/' + offsetY;
}


function Aclick(a)
{
	var url = a.href;
	window.location = url;
	return false;
}

//-->
</script>

<?
}
?>