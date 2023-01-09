UPDATE `creature_template` SET `ScriptName` = 'npc_defier_draghar' WHERE `entry`=187897;

DELETE FROM `creature_text` WHERE `CreatureID`=187897;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(187897, 0, 0, 'If only you could have understood the righteousness of our cause.', 12, 0, 100, 669, 0, 203351, 217506, 0, 'Defier Draghar to Ruby Sitter'),
(187897, 1, 0, 'Such a pointless death.', 12, 0, 100, 669, 0, 203352, 218215, 0, 'Defier Draghar to Ruby Sitter'),
(187897, 2, 0, 'I will not allow you to disturb Melidrussa\'s work!', 14, 0, 100, 0, 0, 203353, 217656, 0, 'Defier Draghar'),
(187897, 3, 0, 'Melidrussa... I have.. failed...', 12, 0, 100, 0, 0, 203354, 218834, 0, 'Defier Draghar to Player');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry` = 371072;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 371072, 0, 1, 31, 0, 3, 188086, 0, 0, 0, 0, '', 'Spell \'Execution\' can only hit \'Ruby Sitter\'');

DELETE FROM `areatrigger` WHERE `SpawnId`=23;
INSERT INTO `areatrigger` (`SpawnId`, `AreaTriggerId`, `IsServerSide`, `MapId`, `PosX`, `PosY`, `PosZ`, `Orientation`, `PhaseUseFlags`, `PhaseId`, `PhaseGroup`, `Shape`, `ShapeData0`, `ShapeData1`, `ShapeData2`, `ShapeData3`, `ShapeData4`, `ShapeData5`, `ShapeData6`, `ShapeData7`, `ScriptName`, `Comment`) VALUES
(23, 23, 1, 2521, 1589.87, -143.295, 126.636, 5.56396, 0, 0, 0, 6, 20, 50, 0, 0, 0, 0, 0, 0, 'areatrigger_start_defier_draghar_intro', 'Ruby Life Pools - Trigger Defier Draghar Intro');

DELETE FROM `areatrigger_template` WHERE `Id`=23 AND `IsServerSide`=1;
INSERT INTO `areatrigger_template` (`Id`, `IsServerSide`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `VerifiedBuild`) VALUES
(23, 1, 6, 0, 20, 50, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (372047, 385281);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(372047, 'spell_ruby_life_pools_steel_barrage'),
(385281, 'spell_ruby_life_pools_molten_steel');
