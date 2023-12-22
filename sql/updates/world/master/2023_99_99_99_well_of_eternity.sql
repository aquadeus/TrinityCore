SET @CGUID := 80000000;
SET @OGUID := 80000000;

-- Instance
UPDATE `instance_template` SET `script` = 'instance_well_of_eternity' WHERE `map` = 939;

-- Creature
DELETE FROM `creature` WHERE `guid` = @CGUID+0;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `VerifiedBuild`) VALUES
(@CGUID+0, 57864, 939, 5788, 5993, '2', '0', 0, 0, 0, 3242.23779296875, -4975.25537109375, 195.4101104736328125, 4.363323211669921875, 7200, 0, 0, 5708, 1067, 0, NULL, NULL, NULL, NULL, 52485); -- Alurmi (Area: Courtyard of Lights - Difficulty: 0) CreateObject1 (Auras: 49414 - Generic Quest Invisibility 1)

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+0;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID+0, 0, 0, 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, '49414'); -- Alurmi - 49414 - Generic Quest Invisibility 1

DELETE FROM `creature_addon` WHERE `guid` = 358828;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(358828, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, '105915 110231'); -- Illidan Stormrage - 105915 - Shadowcloak

UPDATE `creature_template` SET `ScriptName` = 'npc_woe_nozdormu' WHERE `entry` = 55624;
UPDATE `creature_template` SET `BaseAttackTime`=2001980, `unit_flags`=2181071616, `unit_flags2`=4194304, `unit_flags3`=1, `ScriptName`='boss_peroth_arn' WHERE `entry`=55085; -- Peroth'arn
UPDATE `creature_template` SET `unit_flags`=64, `unit_flags2`=4196352, `ScriptName`='npc_woe_legion_demon' WHERE `entry`=55503; -- Legion Demon
UPDATE `creature_template_addon` SET `VisFlags`=1 WHERE `entry`=57864; -- 57864 (Alurmi) - Generic Quest Invisibility 1
UPDATE `creature_template` SET `unit_flags3`=1 WHERE `entry`=56389; -- Shadowcloak Illidan Helper Stalker PH
UPDATE `creature_template` SET `unit_flags3`=1 WHERE `entry`=55154; -- Shadowcloak Helper Stalker PH
UPDATE `creature_template` SET `unit_flags`=32768, `unit_flags2`=2099200 WHERE `entry`=55500; -- Illidan Stormrage

DELETE FROM `creature_template_gossip` WHERE (`CreatureID`=55624 AND `MenuID`=13412);
INSERT INTO `creature_template_gossip` (`CreatureID`, `MenuID`, `VerifiedBuild`) VALUES
(55624, 13412, 52485); -- Nozdormu

DELETE FROM `gossip_menu` WHERE (`MenuID`=13412 AND `TextID`=18851);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`) VALUES
(13412, 18851, 52485); -- 55624 (Nozdormu)

DELETE FROM `creature_template_difficulty` WHERE (`DifficultyID`=2 AND `Entry` = 57864);
INSERT INTO `creature_template_difficulty` (`Entry`, `DifficultyID`, `HealthScalingExpansion`, `HealthModifier`, `ManaModifier`, `CreatureDifficultyID`, `TypeFlags`, `TypeFlags2`) VALUES
(57864, 2, 3, 2, 1, 26030, 0, 0); -- Alurmi

DELETE FROM `creature_template_addon` WHERE `entry` IN (55654, 56389);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(55654, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, '110048 107877'), -- 55654 (Corrupted Arcanist) - Corrupted Night Elf Eyes, Disciplined Caster
(56389, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '105924'); -- 56389 (Shadowcloak Illidan Helper Stalker PH) - Shadowcloak Illidan Stalker Cosmetic


DELETE FROM `creature_template_movement` WHERE `CreatureId`=56389;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Chase`, `Random`, `InteractionPauseTimer`) VALUES
(56389, NULL, NULL, 1, 1, NULL, NULL, NULL);

UPDATE `creature` SET `StringId` = 'legion_demon_woe_intro' WHERE `guid` = 358739;
UPDATE `creature` SET `StringId` = 'dreadlord_defender_woe_intro_1' WHERE `guid` = 358724;
UPDATE `creature` SET `StringId` = 'dreadlord_defender_woe_intro_2' WHERE `guid` = 358728;
UPDATE `creature` SET `StringId` = 'corrupted_arcanist_woe_intro' WHERE `guid` = 358730;
UPDATE `creature` SET `StringId` = 'peroth_arn_woe_intro' WHERE `guid` = 358741;

DELETE FROM `creature_text` WHERE `CreatureID` IN (55624, 55085);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(55624, 0, 0, 'I see you\'ve arrived. This is the eve of the sundering, when the collapse of the Well of Eternity fractured the continents of the world.', 12, 0, 100, 0, 0, 25961, 55271, 0, 'Nozdormu to Player'),
(55624, 1, 0, 'Here, we will snatch up the Dragon Soul before it is lost to the mists of time.', 12, 0, 100, 0, 0, 25962, 55304, 0, 'Nozdormu to Player'),
(55624, 2, 0, 'But first, you must bring down the protective wards of Azshara\'s Highborne lackeys. You will find them within the palace. I will scout on ahead.', 12, 0, 100, 0, 0, 25963, 55305, 0, 'Nozdormu to Player'),
(55624, 3, 0, 'Good luck, heroes!', 12, 0, 100, 0, 0, 25964, 55306, 0, 'Nozdormu to Player'),
(55085, 0, 0, 'He is near, lurking in the shadows... I can sense it.', 12, 0, 100, 0, 0, 26118, 54683, 2, 'Peroth\'arn to Player'),
(55085, 1, 0, 'You, Felguard.  Hold this area.', 12, 0, 100, 0, 0, 26120, 54685, 2, 'Peroth\'arn to Player'),
(55085, 2, 0, 'The rest of you, secure the courtyard.', 12, 0, 100, 0, 0, 26121, 54687, 2, 'Peroth\'arn to Player');

DELETE FROM `vehicle_template_accessory` WHERE (`entry`=55500 AND `seat_id`=0);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(55500, 56389, 0, 1, 'Illidan Stormrage - Shadowcloak Illidan Helper Stalker PH', 8, 0); -- Illidan Stormrage - Shadowcloak Illidan Helper Stalker PH

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=55500 AND `spell_id`=46598;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(55500, 46598, 0, 0);

-- Areatriggers
DELETE FROM `areatrigger_scripts` WHERE `entry`IN (7387, 7029);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7387, 'at_woe_nozdormu_intro'),
(7029, 'at_woe_perotharn_intro');

-- GameObjects
UPDATE `gameobject` SET `state` = 0 WHERE `guid` = 224473;

-- Spell
DELETE FROM `spell_script_names` WHERE `spell_id` IN (105247, 105243);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(105247,'spell_woe_summon_fel_firewall_cosmetic_ph'),
(105243,'spell_woe_summon_fire_wall');

-- Waypoints
SET @PATH_1 := (358724 * 10) << 3;
DELETE FROM `waypoint_data` WHERE `id`= @PATH_1;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES
(@PATH_1, 0, 3196.999, -4934.864, 190.3811, NULL, 0),
(@PATH_1, 1, 3195.483, -4926.509, 194.1676, NULL, 0),
(@PATH_1, 2, 3194.852, -4918.588, 194.3566, NULL, 0),
(@PATH_1, 3, 3199.403, -4912.055, 194.3566, NULL, 0),
(@PATH_1, 4, 3206.897, -4907.776, 194.3566, NULL, 0),
(@PATH_1, 5, 3215.504, -4905.563, 194.1677, NULL, 0),
(@PATH_1, 6, 3224.552, -4905.381, 190.6194, NULL, 0),
(@PATH_1, 7, 3233.596, -4905.18, 185.7695, NULL, 0),
(@PATH_1, 8, 3242.761, -4905.245, 180.9809, NULL, 0),
(@PATH_1, 9, 3249.303, -4905.292, 181.0348, NULL, 0);

SET @PATH_2 := (358728 * 10) << 3;
DELETE FROM `waypoint_data` WHERE `id`= @PATH_2;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES
(@PATH_2, 0, 3189.994, -4935.834, 189.8608, NULL, 0),
(@PATH_2, 1, 3188.416, -4926.269, 194.1676, NULL, 0),
(@PATH_2, 2, 3188.998, -4914.624, 194.3566, NULL, 0),
(@PATH_2, 3, 3195.964, -4905.876, 194.3566, NULL, 0),
(@PATH_2, 4, 3205.412, -4900.862, 194.3566, NULL, 0),
(@PATH_2, 5, 3215.362, -4898.493, 194.1677, NULL, 0),
(@PATH_2, 6, 3224.41, -4898.311, 190.6954, NULL, 0),
(@PATH_2, 7, 3233.646, -4898.109, 185.7427, NULL, 0),
(@PATH_2, 8, 3242.812, -4898.174, 180.9809, NULL, 0),
(@PATH_2, 9, 3249.353, -4898.221, 180.9809, NULL, 0);

SET @PATH_3 := (358730 * 10) << 3;
DELETE FROM `waypoint_data` WHERE `id`= @PATH_3;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES
(@PATH_3, 0, 3197.919, -4941.1826, 189.7942, NULL, 0),
(@PATH_3, 1, 3195.669, -4938.9326, 189.7942, NULL, 0),
(@PATH_3, 2, 3193.669, -4935.4326, 189.7942, NULL, 0),
(@PATH_3, 3, 3192.169, -4924.4326, 194.5442, NULL, 0),
(@PATH_3, 4, 3192.669, -4915.9326, 194.5442, NULL, 0),
(@PATH_3, 5, 3196.919, -4909.4326, 194.5442, NULL, 0),
(@PATH_3, 6, 3206.169, -4904.4326, 194.5442, NULL, 0),
(@PATH_3, 7, 3217.169, -4901.9326, 194.5442, NULL, 0),
(@PATH_3, 8, 3234.169, -4901.6826, 187.5442, NULL, 0),
(@PATH_3, 9, 3252.863, -4901.7812, 180.9809, NULL, 0);

SET @PATH_4 := (358739 * 10) << 3;
DELETE FROM `waypoint_data` WHERE `id`= @PATH_4;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES
(@PATH_4, 0, 3186.113, -4938.482, 189.5661, NULL, 0),
(@PATH_4, 1, 3193.363, -4939.232, 189.5661, NULL, 0),
(@PATH_4, 2, 3195.613, -4940.482, 189.5661, NULL, 0),
(@PATH_4, 3, 3196.917, -4942.767, 189.5242, NULL, 0);
