<?
    if (count($show_files)>0 || access_enable($STORAGE, $type, 'file-attach')) {
        ?>
            <fieldset>
                <legend><?= $legend_title; ?></legend>
                <div id="attach-toolbar" class="toolbar right">
                <?
                    if ($obj2attach->CheckActionEnable($STORAGE, 'file-attach')) {
                        ?>
                            <button id="attach" onclick="return FileAttachAction('<?= htmlspecialchars($type); ?>', <?= $item_id; ?>);"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Присоединить файлы">Присоединить файлы</button>
                        <?
                    } else {
                        ?>
                            <button id="attach" disabled><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Присоединить файлы">Присоединить файлы</button>
                        <?
                    }
                ?>
                </div>


                <table cellpadding="3" cellspacing="1" border="0" width="100%" class="list">
                <tr>
                    <th>Название</th>
                </tr>
                <?
                    for ($i=0;$i<count($show_files);$i++) {
                        ?>
                            <tr>
                                <td><a href="<?= SITE_PROJECT_HTTP_ROOT.$show_files[$i]['fullpath']; ?>" target="_blank"><?= htmlspecialchars($show_files[$i]['name']); ?></a></td>
                            </tr>
                        <?
                    }
                ?>
                </table>
            </fieldset>
        <?
    }

