

<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post">
<input type="hidden" name="report" value="1" />
<input type="hidden" name="report_save" value="0" >
<input type="hidden" name="doc_list" value=""/>
<table cellpadding="0" cellspacing="6" border="0" width="100%" class="tableForm">
   
    <tr>
        <td width="10%">
            <table cellpadding="0" cellspacing="6" border="0" width="40%">
                <tr>
                    <td colspan="3" nowrap>с
                        <?
                            $start_date_str = '';
                            if ($start_date!='') {
                                $start_date_str = format_date('{d}.{m}.{Y}', $start_date);
                                if ($start_date_str=='00.00.0000') $start_date_str = '';
                            }
                        ?>
                        <input type="text" name="start_date" id="start_date" value="<?= htmlspecialchars($start_date_str); ?>" class="date center" />
                        <a href="javascript:;" onclick="selectDate(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" /></a>
                        по
                        <?
                            $stop_date_str = '';
                            if ($stop_date!='') {
                                $stop_date_str = format_date('{d}.{m}.{Y}', $stop_date);
                                if ($stop_date_str=='00.00.0000') $stop_date_str = '';
                            }
                        ?>
                        <input type="text" name="stop_date" id="stop_date" value="<?= htmlspecialchars($stop_date_str); ?>" class="date center" />
                        <a href="javascript:;" onclick="selectDate2(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" /></a>
                      
                        
                        
                   
                <table border="0" cellspacing="0" cellpadding="2" width="200" class="tableForm toolbar">
                <tr>
                    <td><button onclick="setDates(1,1);return false;">Янв.</button></td>
                    <td><button onclick="setDates(2,1);return false;">Фев.</button></td>
                    <td><button onclick="setDates(3,1);return false;">Мар.</button></td>
                    <td><button onclick="setDates(1,3);return false;">I<br/>кв.</button></td>
                    <td rowspan="2"><button onclick="setDates(1,6);return false;">I<br/>пол.</button></td>
                </tr>
                <tr>
                    <td><button onclick="setDates(4,1);return false;">Апр.</button></td>
                    <td><button onclick="setDates(5,1);return false;">Май</button></td>
                    <td><button onclick="setDates(6,1);return false;">Июн.</button></td>
                    <td><button onclick="setDates(4,3);return false;">II<br/>кв.</button></td>
                </tr>
                <tr>
                    <td><button onclick="setDates(7,1);return false;">Июл.</button></td>
                    <td><button onclick="setDates(8,1);return false;">Авг.</button></td>
                    <td><button onclick="setDates(9,1);return false;">Сен.</button></td>
                    <td><button onclick="setDates(7,3);return false;">III<br/>кв.</button></td>
                    <td rowspan="2"><button onclick="setDates(7,6);return false;">II<br/>пол.</button></td>
                </tr>
                <tr>
                    <td><button onclick="setDates(10,1);return false;">Окт.</button></td>
                    <td><button onclick="setDates(11,1);return false;">Ноя.</button></td>
                    <td><button onclick="setDates(12,1);return false;">Дек.</button></td>
                    <td><button onclick="setDates(10,3);return false;">IV<br/>кв.</button></td>
                </tr>
                </table>
                <br/>
            </td>
                
                </tr>
                
                 <tr>
        <td >
            <table cellpadding="0" cellspacing="0" border="0" width="750">
                <tr>
                    <td width="38%">
                        <select id="rep" name="report_employees[]" size="15" class="wide" multiple>
                            <?
                                for ($i=0;$i<count($all_employees->_collection);$i++) {
                                    $employee = $all_employees->_collection[$i];
                                    if (!key_exists($employee->id_employee, $arr)) continue;
                                    ?>
                                        <option value="<?= $employee->id_employee."-".$employee->department_id; ?>"><?= htmlspecialchars($employee->name); ?></option>
                                    <?
                                }
                            ?>
                        </select>
                    </td>
                    <td valign="middle" align="center" width="10%">
                        <table>
                          <tr>
                            <td><input type=button value="&gt;" onclick="moveOption(document.editform.elements['report_employees[]'], document.editform.elements['all_employees[]'])" class="arrowButton" /></td>
                            <td><input type=button value="&lt;" onclick="moveOption(document.editform.elements['all_employees[]'], document.editform.elements['report_employees[]'])" class="arrowButton" /></td>
                          </tr>
                          <tr>
                            <td><input type=button value="&gt;&gt;" onclick="moveAllOptions(document.editform.elements['report_employees[]'], document.editform.elements['all_employees[]'])" class="arrowButton" /></td>
                            <td><input type=button value="&lt;&lt;" onclick="moveAllOptions(document.editform.elements['all_employees[]'], document.editform.elements['report_employees[]'])" class="arrowButton" /></td>
                          </tr>
                        </table>
                    </td>
                    <td width="38%">
                        <select id="all" name="all_employees[]" size="15" class="wide" multiple>
                            <?
                                for ($i=0;$i<count($all_employees->_collection);$i++) {
                                    $employee = $all_employees->_collection[$i];
                                    if (key_exists($employee->id_employee, $arr)) continue;
                                    ?>
                                        <option value="<?= $employee->id_employee."-".$employee->department_id; ?>"><?= htmlspecialchars($employee->name); ?></option>
                                    <?
                                }
                            ?>
                        </select>
                    </td>
                    <td width="14%" valign="middle">
                        <button id="add-department" onclick="return SelectDepartmentAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Добавить подразделение" />Выбрать подразделение</button>
                    </td>
                </tr>
            </table>
        </td>
        <td width="20%" valign="top">&nbsp;</td>
    </tr>
                              
    <tr>
        <td colspan="2">
            <button onclick="SubmitRecord();"><img src="<?= SITE_URI_ROOT; ?>img/iconSearch.gif" title="Вывести отчет" />Вывести отчет</button>
            </td>
          
    </tr>
                
            </table>
        </td>
        <td width="20%" valign="top">&nbsp;</td>
    </tr>
</table>
</form>

<?
    if (!$errors) {
    	?>		  	
    	<?= $page_break;?>
<table id="rep_list" name="rep_list" cellpadding="3" cellspacing="1" border="0" class="list">
    <tr>
        <th>№</th>
        
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/date/sort_dir/<?= $sort=='date' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Дата</a><?= $sort=='date' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
       
        <th> Ф.И.О</th>
        <th> Должность</th>
        <th> Место командировки</th>
        <th> Контактный телефон(или места куда убыл)</th>
        <th> Цель командировки</th>
        <th> Время убытия</th>
        <th> Время прибытия</th>
        <th> Комментарий</th>
        <th> &nbsp;</th>
    </tr>
    <?
        $odd=false;
       	
       	
        for ($i=0; $i<count($coms); $i++) {
        	
        	
            	$odd=!$odd;
            	$empl = new employee();
            	$empl->id_employee = $coms[$i]['employee_id']; 
            	$empl->Load();
            	
            ?>
                <tr <?=($odd?'class="odd"':'')?>>
                    <th><?= $i+1; ?>.</th>
                   
                    <td><?= htmlspecialchars($coms[$i]['date']); ?></td>
                    <td><?= htmlspecialchars($empl->name); ?></td>
                    <td><?= htmlspecialchars($coms[$i]['post']); ?></td>
                                
                    <td><?= htmlspecialchars($coms[$i]['place']); ?></td>
                    <td><?= htmlspecialchars($coms[$i]['phone']); ?> </td>
                    <td><?= htmlspecialchars($coms[$i]['plan']); ?></td>
                     <td><?= htmlspecialchars($coms[$i]['start_time']); ?> </td>
           
                    <td><?= htmlspecialchars($coms[$i]['stop_time']);?></td>
					<td><?= htmlspecialchars($$coms[$i]['description']);?></td>
					</tr>
            <?
        	
        }
    ?>
</table>
    		   	
    
        <?
        
    }
?>



<? include SITE_FILE_ROOT.'template/ocalendar.php'; ?>
<script type="text/javascript" language="JavaScript">
 setMovableSelect(document.editform.elements['all_employees[]'], true);
    setMovableSelect(document.editform.elements['report_employees[]'], true);

function SubmitRecord() {
    selectAllLists(document.editform.elements['report_employees[]']);
    document.getElementById('editform').submit();
}




function selectDate(event)
{
        var X=event.clientX+16;
        var Y=event.clientY+16;
        if (X+185>parseInt(document.body.clientWidth)) X=X-185;
        if (Y+215>parseInt(document.body.clientHeight)) Y=Y-215;
        YAHOO.yui.oCalendar.open(document.editform["start_date"].value,"",X,Y,setDate);
}
    
    
function selectDate2(event)
{
        var X=event.clientX+16;
        var Y=event.clientY+16;
        if (X+185>parseInt(document.body.clientWidth)) X=X-185;
        if (Y+215>parseInt(document.body.clientHeight)) Y=Y-215;
        YAHOO.yui.oCalendar.open(document.editform["stop_date"].value,"",X,Y,setDate2);
}    


function setDate(sDate)
{
        document.editform["start_date"].value = sDate;
}
    
function setDate2(sDate)
{
        document.editform["stop_date"].value = sDate;
}

 function setDates(month,nMonth) {
        var oDate=new Date;
        oDate.setMonth(month-1);
        oDate.setDate(1);
        var year = oDate.getFullYear(), day = oDate.getDate();
        if (month<10) month = '0' + month;
        if (day<10) day = '0' + day;
        var sDate = day + '.' + month + '.' + year;
        document.editform["start_date"].value = sDate;
        oDate.setMonth(oDate.getMonth()+nMonth);
        oDate.setDate(0);
        year = oDate.getFullYear(); month = (oDate.getMonth()+1); day = oDate.getDate();
        if (month<10) month = '0' + month;
        if (day<10) day = '0' + day;
        var sDate = day + '.' + month + '.' + year;
        document.editform["stop_date"].value = sDate;
    }

    
function hideCalendar(){
        YAHOO.yui.oCalendar.close();
}

var DepartmentWindow=null;

function SelectDepartmentAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>department/select-department/callback/SetDepartment';

    var w = 550;
    var h = 200;
    var param = '';
    if(DepartmentWindow==null || DepartmentWindow.closed){
    	selectAllLists(document.editform.elements['report_employees[]']);
	    moveOption(document.editform.elements['report_employees[]'], document.editform.elements['all_employees[]']);
        DepartmentWindow=OpenPopupWindow(url, w, h, param);
    }else{
        DepartmentWindow.focus();
    }
    
    return false;
}


function SetDepartment(id, text)
{
	
	//department_id maybe NULL!!
	for(i=0;i<document.editform['all_employees[]'].length;i++)
	{
		var s = document.editform['all_employees[]'].options[i].value;
		
		var ind = s.indexOf('-');
		
		s = s.substring(ind+1);
		
		if(s == id)
		{
			document.editform['all_employees[]'].options[i].selected = true;
		}
	}
	
	moveOption( document.editform.elements['all_employees[]'], document.editform.elements['report_employees[]']);
	
	
}



function DetailAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'detail')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/detail/id/' + id + '/';
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

</script>