
GO

/****** Object:  View [fct].[vRegionQueriesUpd]    Script Date: 02.02.2026 13:43:30 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO






-- ==================================================================
--Автор( ы ): Блюдов А.В., Никольский Г.Н.
--Дата создания : 13.11.2017 11 : 48
--Дата обновления : 25.01.2016 11 : 48
--Дата обновления : 31.05.2021 14 : 20
-- Дата обновления: 18.04.2022 для ГВЭ уточнён barcodetype = 1
-- Дата обновления: 9.06.2023 В GROUP BY добавил o.[PressOrderID] (А.В.Гор.)
-- Дата обновления: 02.02.2025 избавился от cte
--Описание : Представление для получения кода зоны для региона
--                  и экзамена
--
-- ==================================================================
CREATE VIEW[fct].[vRegionQueriesUpd] AS

SELECT rqg.RegionQueryGroupID, 
	eq.ExamQueryID, 
	rqg.ExamGlobalID AS ExamID, 
	ep.PackVolume1, 
	ep.PackVolume2,
	ep.PackVolume1*eq.Quantity1 + ep.PackVolume2*eq.Quantity2 AS CalculatedQuantity,
--	eq.SendedQuantity - ep.PackVolume1*eq.Quantity1 - ep.PackVolume2*eq.Quantity2 AS CheckQuantity,
	rqg.OrderStatusCode,
	CASE WHEN s.TaskAmountD <> 0 and NOT (eq.TestTypeCode = 6 AND eq.SubjectCode = 4)
		THEN 1
	WHEN eq.TestTypeCode = 5
		THEN 2
	WHEN eq.SubjectCode = 22 and eq.TestTypeCode = 4
		THEN 3
	ELSE 0 END AS BarcodeType,
	po.PressOrderID, po.OrderedQuantity1, po.OrderedQuantity2,
	CONVERT( VARCHAR( 30 ), rqg.CreateDate, 104 ) AS QueryCreateDate,
	eq.REGION, r.RegionName, z.ZoneCode,
	eq.SubjectCode, s.SubjectName, eq.ExamDate,
	eq.Quantity1,
	eq.Quantity2, 
	eq.SendedQuantity,
	rq.WaveCode, 
	rq.Packs, 
	rq.TechnologyCode,
	COALESCE( po.State, 0 ) AS State, 
	COALESCE( po.LoadNo, 0 ) AS LoadNo,
	CONVERT( varchar, po.CreateDate, 104 ) AS OrderCreateDate,
	fct.NoOfPdfVariantsNew( eq.SubjectCode, eq.ExamDate, z.ZoneCode, rq.TechnologyCode ) AS VariantCount,
	fct.PdfNameForPrintNew( eq.SubjectCode, eq.ExamDate, z.ZoneCode, rq.TechnologyCode )  AS PdfName
FROM[dbo].[scp_ExamQueries] AS eq
JOIN[dbo].[scp_RegionQueries] AS rq ON( RegionQueryFK = RegionQueryID )
JOIN[dbo].[scp_RegionQueryGroups] AS rqg ON( eq.RegionQueryGroupID = rqg.RegionQueryGroupID )
JOIN[dbo].[scp_ExamParams] ep ON( rqg.ExamGlobalID = ep.ExamID and rqg.TechnologyCode = ep.TechnologyCode )
JOIN[dbo].[sc_Subjects] s ON( s.SubjectCode = eq.SubjectCode )
JOIN dbo.rbdc_Regions AS r ON r.REGION = rq.REGION
JOIN[zone].[ftcv_Zones] AS z ON rqg.ExamGlobalID = z.[ExamID] and z.RegionCode = rq.Region and z.TechnologyCode = rq.TechnologyCode

LEFT JOIN[fct].[PressOrders] AS po ON rq.RegionQueryGroupID = po.RegionQueryGroupID AND eq.ExamQueryID = po.ExamQueryID
WHERE eq.SendedQuantity > 0 AND rqg.OrderStatusCode = 100




GO


