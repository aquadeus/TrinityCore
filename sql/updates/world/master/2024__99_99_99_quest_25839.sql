UPDATE `quest_template_locale` SET `QuestDescription`='El Cataclismo ha acercado al enemigo a nuestros hogares, así que el jefe de las operaciones especiales de Gnomeregan me ha pedido que construya un dispositivo para enviar al personal de forma rápida y sigilosa a cualquier rincón de Dun Morogh.$b$b¡Contemplad el lanzador personal ultraseguro!$b$bCon una descarga eléctrica y las ventajas de un sistema de paracaídas DMT, despliegue muy tardío para los no entendidos, este pequeño te llevará al frente en un santiamén. ¡Súbete a bordo y prepárate para el vuelo! Slamp Tiembladiente te estará esperando en la zona de aterrizaje.', `VerifiedBuild`=53989 WHERE (`ID`=25839 AND `locale`='esES');
UPDATE `quest_template_locale` SET `VerifiedBuild`=53989 WHERE (`locale`='esES' AND `ID` IN (46736,46735,46277));

DELETE FROM `quest_details` WHERE `ID`=25839;
INSERT INTO `quest_details` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `VerifiedBuild`) VALUES
(25839, 1, 25, 5, 0, 0, 0, 0, 0, 53989); -- The Ultrasafe Personnel Launcher

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup`=41398 AND `SourceEntry`=77424;
INSERT INTO `conditions` VALUES (18, 41398, 77424, 0, 0, 9, 0, 25839, 0, 0, '', 0, 0, 0, '', 'Npc 41398 requires quest 25839 active for spellclick');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (41398, 41327);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(41398, 77424, 1, 0),
(41327, 77424, 1, 0);

UPDATE `creature_template` SET `npcflag`=16777216, `ScriptName`='npc_ultrasafe_personnel_launcher' WHERE `entry`=41398; -- Ultrasafe Personnel Launcher
