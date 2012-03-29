<br/>
<form method=post class="formz" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>">
    <input type="hidden" name="save" value="1" />
    <table align="left" cellpadding=3 cellspacing=1 width="750" border="0" class="list">
    <tbody>
        <tr>
            <td><b> Количество объявлений выводимых в предосмотре, на главной странице</b></td>
            <td><input name="am_news" value="<?=$news_conf->am_news?>" maxlength=2></td>
        </tr>
        <tr>
            <td><b> Количество первых символов объявления в предосмотре</b></td>
            <td><input name="PreLengthC" value="<?=$news_conf->PreLengthC?>" maxlength=5></td>
        </tr>
        <tr>
            <td><b> Ширина таблицы объявлений (в процентах - 75% или пикселях - 800)</b></td>
            <td><input name="tab_width" value="<?=$news_conf->tab_width?>" maxlength=5></td>
        </tr>
        <tr>
            <td><b> Показывать время добавления объявления?</b></td>
            <td><input type="radio" name="p_time" value="1"<?if ($news_conf->p_time==1){print "checked";}?>> да 
                <input type="radio" name="p_time" value="2"<?if ($news_conf->p_time==2){print "checked";}?>> нет
            </td>
        </tr>
        <tr>
            <td><b> Показывать дату добавления объявления?</b></td>
            <td><input type="radio" name="p_date" value="1" <?if ($news_conf->p_date==1){print "checked";}?>> да 
                <input type="radio" name="p_date" value="2" <?if ($news_conf->p_date==2){print "checked";}?>> нет
            </td>
        </tr>
        <tr><td align=center colspan=2 class=buten><b>Настройки графики</b></td></tr>
        <tr>
            <td><b> Высота превью картинки</b> (ширина меняется соответственно масштабу)</td>
            <td><input name=img_height value="<?=$news_conf->img_height?>" maxlength=5></td>
        </tr>
        <tr>
            <td colspan="2" class="toolbar">
                <button type="submit" class="button" >Сохранить &raquo;</button>  
            </td>
        </tr>
        </tbody>
    </table>
</form>


