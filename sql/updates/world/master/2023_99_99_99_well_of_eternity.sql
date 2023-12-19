SET @CGUID := 80000000;
SET @OGUID := 80000000;

UPDATE `instance_template` SET `script` = 'instance_well_of_eternity' WHERE `map` = 939;
UPDATE `creature_template` SET `ScriptName` = 'instance_well_of_eternity' WHERE `entry` = 55624;
UPDATE `creature_template` SET `ScriptName` = 'instance_well_of_eternity' WHERE `entry` = 55085;

DELETE FROM `creature_template_gossip` WHERE (`CreatureID`=55624 AND `MenuID`=13412);
INSERT INTO `creature_template_gossip` (`CreatureID`, `MenuID`, `VerifiedBuild`) VALUES
(55624, 13412, 52485); -- Nozdormu

DELETE FROM `gossip_menu` WHERE (`MenuID`=13412 AND `TextID`=18851);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`) VALUES
(13412, 18851, 52485); -- 55624 (Nozdormu)

DELETE FROM `areatrigger_scripts` WHERE `entry`IN (7387, 7029);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7387, 'at_woe_nozdormu_intro'),
(7029, 'at_woe_perotharn_intro');

DELETE FROM `creature_text` WHERE `CreatureID` IN (55624, 55085);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(55624, 0, 0, 'I see you\'ve arrived. This is the eve of the sundering, when the collapse of the Well of Eternity fractured the continents of the world.', 12, 0, 100, 0, 0, 25961, 55271, 0, 'Nozdormu to Player'),
(55624, 1, 0, 'Here, we will snatch up the Dragon Soul before it is lost to the mists of time.', 12, 0, 100, 0, 0, 25962, 55304, 0, 'Nozdormu to Player'),
(55624, 2, 0, 'But first, you must bring down the protective wards of Azshara\'s Highborne lackeys. You will find them within the palace. I will scout on ahead.', 12, 0, 100, 0, 0, 25963, 55305, 0, 'Nozdormu to Player'),
(55624, 3, 0, 'Good luck, heroes!', 12, 0, 100, 0, 0, 25964, 55306, 0, 'Nozdormu to Player'),
(55085, 0, 0, 'He is near, lurking in the shadows... I can sense it.', 12, 0, 100, 0, 0, 26118, 54683, 0, 'Peroth\'arn to Player'),
(55085, 1, 0, 'You, Felguard.  Hold this area.', 12, 0, 100, 0, 0, 26120, 54685, 0, 'Peroth\'arn to Player'),
(55085, 2, 0, 'The rest of you, secure the courtyard.', 12, 0, 100, 0, 0, 26121, 54687, 0, 'Peroth\'arn to Player');

DELETE FROM `creature` WHERE `guid` = @CGUID+0;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `VerifiedBuild`) VALUES
(@CGUID+0, 57864, 939, 5788, 5993, '2', '0', 0, 0, 0, 3242.23779296875, -4975.25537109375, 195.4101104736328125, 4.363323211669921875, 7200, 0, 0, 5708, 1067, 0, NULL, NULL, NULL, NULL, 52485); -- Alurmi (Area: Courtyard of Lights - Difficulty: 0) CreateObject1 (Auras: 49414 - Generic Quest Invisibility 1)

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+0;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID+0, 0, 0, 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, '49414'); -- Alurmi - 49414 - Generic Quest Invisibility 1

UPDATE `creature_template_addon` SET `VisFlags`=1 WHERE `entry`=57864; -- 57864 (Alurmi) - Generic Quest Invisibility 1

DELETE FROM `creature_template_difficulty` WHERE (`DifficultyID`=2 AND `Entry` = 57864);
INSERT INTO `creature_template_difficulty` (`Entry`, `DifficultyID`, `HealthScalingExpansion`, `HealthModifier`, `ManaModifier`, `CreatureDifficultyID`, `TypeFlags`, `TypeFlags2`) VALUES
(57864, 2, 3, 2, 1, 26030, 0, 0); -- Alurmi

UPDATE `gameobject` SET `state` = 0 WHERE `guid` = 224473;
