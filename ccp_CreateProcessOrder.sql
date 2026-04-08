GO

/****** Object:  StoredProcedure [fct].[ccp_NewPressOrders]    Script Date: 01.02.2026 19:30:48 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO



-- ==================================================================
-- Автор(ы):        Никольский Г.Н.
-- Дата создания:   12.03.2021 11:38
-- Описание:        Создание записи в PressOrders. Уточнение - надо ли 
-- создать пробник. Лог в PressSessions
-- Дата обновления: 02.02.2025 добавлено вычисление OrderedQuantity в случае несовпадения с Packs
-- ==============================================

CREATE PROCEDURE [fct].[ccp_CreatePressOrders] 
@TechnologyCode as int,
@RegionQueryGroupID as varchar(40),
@ExamQueryID as varchar(40),
@REGION as int, 
@State as int,
@OrderedQuantity1 as int,
@OrderedQuantity2 as int,
@InPack1 as int, 
@InPack2 as int, 
@SubjectCode as int, 
@BarcodeType as int,
@Probe as int, 
@ExamDate as varchar(20),
@Year as int, 
@Zone as int, 
@LoadNo  as int,
@IP as varchar(50),
@Port as int,
@StationCode as int,
@OperatorCode as int,
@Packs as int,
@Total as int
 
AS
DECLARE @PressOrderID as int
DECLARE @Probe1 as int
DECLARE @probe2 as int
DECLARE @q1 AS int
DECLARE @q2 as int

INSERT INTO fct.PressOrders(RegionQueryGroupID, ExamQueryID, REGION, State, OrderedQuantity1, OrderedQuantity2, InPack1, InPack2, SubjectCode, BarcodeType, 
							Probe, ExamDate, Year, Zone, LoadNo) 
VALUES(@RegionQueryGroupID, @ExamQueryID, @REGION, @State, @OrderedQuantity1, @OrderedQuantity2, @InPack1, @InPack2, @SubjectCode, @BarcodeType, 
		@Probe, @ExamDate, @Year, @Zone, @LoadNo)
set @PressOrderID = @@identity

INSERT INTO fct.PressSessions(PressOrderID,IPAddress,Port,OpTime, StationCode, OperatorCode,State, loadNo)
VALUES(@PressOrderID,@IP, @port, GETDATE(), @StationCode, @OperatorCode, @State, @LoadNo )

set @q1 = @OrderedQuantity1
set @q2 = @OrderedQuantity1

IF @InPack1<> 0 AND @InPack2 <> 0 AND @OrderedQuantity1 + @OrderedQuantity2 <> @Packs
	BEGIN
		SET @q2 = (@Total	- @InPack1*@Packs) / (@InPack2- @InPack1)
		SET @q1 = @Packs - @OrderedQuantity2
		UPDATE fct.PressOrders set OrderedQuantity1 = @q1, OrderedQuantity2 = @q2 where PressOrderID = @PressOrderID
	END
ELSE 
	BEGIN	
		IF @InPack1<> 0 AND @OrderedQuantity1 <> @Packs
		BEGIN
			SET @OrderedQuantity1 = @Packs
			UPDATE fct.PressOrders set OrderedQuantity1 = @q1 where PressOrderID = @PressOrderID
		END
		IF  @InPack2<> 0 AND @OrderedQuantity2 <> @Packs
		BEGIN
			SET @OrderedQuantity2 = @Packs
			UPDATE fct.PressOrders set OrderedQuantity2 = @q2 where PressOrderID = @PressOrderID
		END
	END

IF @OrderedQuantity1 <> 0 AND NOT EXISTS (SELECT 1 FROM fct.PressOrders AS p JOIN dbo.scp_RegionQueryGroups AS r ON(p.RegionQueryGroupID = r.RegionQueryGroupID)
				WHERE r.TechnologyCode = @TechnologyCode AND p.Zone = @Zone AND p.ExamDate = @ExamDate AND p.SubjectCode = @SubjectCode AND OrderedQuantity1<>0 AND probe = 1)
	SET @Probe1 = 1
ELSE
	SET @Probe1 = 0
		
IF @OrderedQuantity2 <> 0 AND NOT EXISTS (SELECT 1 FROM fct.PressOrders AS p JOIN dbo.scp_RegionQueryGroups AS r ON(p.RegionQueryGroupID = r.RegionQueryGroupID)
				WHERE r.TechnologyCode = @TechnologyCode AND p.Zone = @Zone AND p.ExamDate = @ExamDate AND p.SubjectCode = @SubjectCode AND OrderedQuantity2<>0 AND probe = 1)
	SET @Probe2 = 2
ELSE
	SET @Probe2 = 0

IF @Probe1 + @Probe2 = 1 
	BEGIN 
		UPDATE fct.PressOrders set OrderedQuantity1 = OrderedQuantity1 + 1, probe = 1 where PressOrderID = @PressOrderID
		SET @OrderedQuantity1 = @OrderedQuantity1 + 1
	END
IF @Probe1 + @Probe2 = 2 
	BEGIN
		UPDATE fct.PressOrders set OrderedQuantity2 = OrderedQuantity2 + 1, probe = 1 where PressOrderID = @PressOrderID
		SET @OrderedQuantity2 = @OrderedQuantity2 + 1
	END
IF @Probe1 + @Probe2 = 3 
	BEGIN 
		UPDATE fct.PressOrders set OrderedQuantity1 = OrderedQuantity1 + 1, OrderedQuantity2 = OrderedQuantity2 + 1, probe = 1 where PressOrderID = @PressOrderID
		SET @OrderedQuantity1 = @OrderedQuantity1 + 1
		SET @OrderedQuantity2 = @OrderedQuantity2 + 1
	END
SELECT @PressOrderID, @Probe1 + @Probe2




GO


