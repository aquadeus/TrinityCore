DELETE FROM `creature_equip_template` WHERE (`ID`=1 AND `CreatureID` = 213217);
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`, `VerifiedBuild`) VALUES
(213217, 1, 168973, 0, 0, 0, 0, 0, 0, 0, 0, 58238); -- Speaker Brokk

DELETE FROM `areatrigger_template` WHERE (`IsCustom`=0 AND `Id` = 35810);
INSERT INTO `areatrigger_template` (`Id`, `IsCustom`, `Flags`, `VerifiedBuild`) VALUES
(35810, 0, 0, 58238);

DELETE FROM `areatrigger_create_properties` WHERE (`IsCustom`=0 AND `Id` IN (35542, 33334));
INSERT INTO `areatrigger_create_properties` (`Id`, `IsCustom`, `AreaTriggerId`, `IsAreatriggerCustom`, `Flags`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `AnimId`, `AnimKitId`, `DecalPropertiesId`, `SpellForVisuals`, `TimeToTargetScale`, `Speed`, `Shape`, `ShapeData0`, `ShapeData1`, `ShapeData2`, `ShapeData3`, `ShapeData4`, `ShapeData5`, `ShapeData6`, `ShapeData7`, `ScriptName`, `VerifiedBuild`) VALUES
(35542, 0, 4520, 0, 4, 0, 0, 0, 0, -1, 0, 1090321266, NULL, 3000, 0, 1, 2.5, 7.5, 7.5, 2.5, 7.5, 7.5, 0, 0, '', 58238), -- Spell: 471646 (Exhaust Vents)
(33334, 0, 35810, 0, 0, 0, 0, 0, 0, -1, 0, 1082130432, NULL, 30000, 19.9996795654296875, 4, 4, 4, 4, 4, 1.5, 1.5, 0, 0, 'at_master_machinists_lava_cannon', 58238); -- Spell: 449167 (Lava Cannon)

SET @CGUID := 100000000;

-- Creature
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+5;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `VerifiedBuild`) VALUES
(@CGUID+0, 214049, 2652, 14883, 14883, '1,2,8,23,205', '0', 0, 0, 0, -205.720489501953125, 277.3663330078125, 351.1593017578125, 2.874309301376342773, 7200, 0, 0, 0, NULL, NULL, NULL, NULL, 58238), -- Vent Stalker (Area: The Stonevault - Difficulty: Mythic) CreateObject1 (Auras: 206150 - Challenger's Might)
(@CGUID+1, 214049, 2652, 14883, 14883, '1,2,8,23,205', '0', 0, 0, 0, -236.23785400390625, 259.888885498046875, 350.745025634765625, 1.336958527565002441, 7200, 0, 0, 0, NULL, NULL, NULL, NULL, 58238), -- Vent Stalker (Area: The Stonevault - Difficulty: Mythic) CreateObject1 (Auras: 206150 - Challenger's Might)
(@CGUID+2, 213660, 2652, 14883, 14883, '1,2,8,23,205', '0', 0, 0, 0, -262.826385498046875, 264.951385498046875, 352.283355712890625, 0.527664899826049804, 7200, 0, 0, 0, NULL, NULL, NULL, NULL, 58238), -- Call Metal Stalker (Area: The Stonevault - Difficulty: Mythic) CreateObject1 (Auras: 206150 - Challenger's Might)
(@CGUID+3, 213660, 2652, 14883, 14883, '1,2,8,23,205', '0', 0, 0, 0, -196.899307250976562, 302.953125, 352.283355712890625, 3.630623579025268554, 7200, 0, 0, 0, NULL, NULL, NULL, NULL, 58238), -- Call Metal Stalker (Area: The Stonevault - Difficulty: Mythic) CreateObject1 (Auras: 206150 - Challenger's Might)
(@CGUID+4, 214049, 2652, 14883, 14883, '1,2,8,23,205', '0', 0, 0, 0, -254.333328247070312, 289.826385498046875, 351.1593017578125, 6.015422344207763671, 7200, 0, 0, 0, NULL, NULL, NULL, NULL, 58238), -- Vent Stalker (Area: The Stonevault - Difficulty: Mythic) CreateObject1 (Auras: 206150 - Challenger's Might)
(@CGUID+5, 214049, 2652, 14883, 14883, '1,2,8,23,205', '0', 0, 0, 0, -222.86285400390625, 308.571197509765625, 351.1593017578125, 4.456800460815429687, 7200, 0, 0, 0, NULL, NULL, NULL, NULL, 58238); -- Vent Stalker (Area: The Stonevault - Difficulty: Mythic) CreateObject1 (Auras: 206150 - Challenger's Might)


UPDATE `creature_template` SET `faction`=14, `speed_run`=1.428571462631225585, `BaseAttackTime`=2000, `unit_flags`=0x2000200, `unit_flags2`=0x800, `unit_flags3`=0x1088001 WHERE `entry`=213694; -- Scrap Block

DELETE FROM `creature_template_difficulty` WHERE (`DifficultyID`=23 AND `Entry` IN (213694,213660,214049,221586,213216,213217));
INSERT INTO `creature_template_difficulty` (`Entry`, `DifficultyID`, `LevelScalingDeltaMin`, `LevelScalingDeltaMax`, `ContentTuningID`, `StaticFlags1`, `StaticFlags2`, `StaticFlags3`, `StaticFlags4`, `StaticFlags5`, `StaticFlags6`, `StaticFlags7`, `StaticFlags8`, `VerifiedBuild`) VALUES
(213694, 23, 0, 0, 1279, 0x0, 0x0, 0x2000000, 0x0, 0x0, 0x0, 0x0, 0x0, 58238), -- 213694 (Scrap Block) - CannotTurn
(213660, 23, 0, 0, 1279, 0x20000100, 0x0, 0x2000000, 0x0, 0x0, 0x0, 0x0, 0x0, 58238), -- 213660 (Call Metal Stalker) - Sessile, Floating - CannotTurn
(214049, 23, 0, 0, 1279, 0x20000100, 0x0, 0x2000000, 0x0, 0x0, 0x0, 0x0, 0x0, 58238), -- 214049 (Vent Stalker) - Sessile, Floating - CannotTurn
(221586, 23, 0, 0, 1279, 0x30000100, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 58238), -- 221586 (Speaker Dorlita) - Sessile, CanSwim, Floating
(213216, 23, 2, 2, 1279, 0x10000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 58238), -- 213216 (Speaker Dorlita) - CanSwim
(213217, 23, 2, 2, 1279, 0x10000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 58238); -- 213217 (Speaker Brokk) - CanSwim

UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=2, `LevelScalingDeltaMax`=2, `ContentTuningID`=1279, `StaticFlags1`=0x10000000, `VerifiedBuild`=58238 WHERE (`Entry`=213217 AND `DifficultyID`=23); -- 213217 (Speaker Brokk) - CanSwim

-- Conditions
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=445541;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ConditionStringValue1`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 445541, 0, 0, 51, 0, 5, 214049, 0, '', 0, 0, 0, '', 'Spell \'Exhaust Vents\' can only hit \'Vent Stalker\'');

