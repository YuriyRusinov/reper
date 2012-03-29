</div>
<?
    if (!(defined('DISABLE_MENU') && DISABLE_MENU)) {
?>
    </td>
    <td id="rightColumn">    
<?
   

    $ids=calendar::getIdsForEmployee($STORAGE['current_employee']->login);
    $ids[]=0;
    $ids=join(',',$ids);

    $where=array();
    $where[]='"calendar_id" in ('.$ids.')';
    $where[]='"notice"=\'yes\'';
    $where[]='"completed"=\'no\'';
    $date = date('Y-m-d');
    $where[]='"start_date"<=\''.$date.'\' AND "stop_date">=\''.$date.'\'';

    $query = 'SELECT COUNT(*) AS num FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ';
    $db->query($query);
    $all_num = $db->value[0]['num'];
    if($all_num>0){
?>
      <div id="alertnotices-organizer" class="alertnotices"><a href="<?=SITE_URI_ROOT?>organizer/day/">Уведомления (ежедневник)</a>: <?= $all_num; ?></div>
<?
    }


    
?>    
<?include SITE_FILE_ROOT.'template/calendar3.php';?> 
    </td>
    </tr>
    </table>
    <table id="footerTable" cellspacing="0" cellpadding="0" border="0" width="100%">
        <tr>
            <td>
                Copyright &copy; 2011 <?= SITE_NAME;?>. Все права защищены
            </td>
        </tr>
    </table>
<?
    };
?>
</body>
</html>