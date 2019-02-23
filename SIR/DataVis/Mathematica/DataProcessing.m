BeginPackage["DataProcessing`"]

Needs["DatabaseLink`"]


getPerson::usage          = "get person data"
getLogData::usage         = "get log data"
PopulationDataPlot::usage = "Plot the populations"
CellDataPlot::usage       = "Plot in-host dynamics"
getConnections::usage     = "Get connections"
getGraph::usage           = "Get graph"

SIRBlue  = RGBColor[0.2, 0.2, 0.7];
SIRRed   = RGBColor[0.7, 0.2, 0.2];
SIRGreen = RGBColor[0.2, 0.7, 0.2];
SIRBlack = RGBColor[0., 0., 0.];


Begin["Private`"]

(*=============================General Data===================================*)
getPerson[{id___Integer}, {props__}] :=
 Module[{query, rawData, data, rule, propList, keys},
  query =
   "SELECT PersonID, Time, " <> (Sequence @@ Riffle[{props}, ", "]) <>
     " FROM PersonValues where PersonID=" <>
    Sequence @@ Riffle[ToString /@ {id}, " or PersonID="];
  rawData = SQLExecute[conn, query];
  If[MatchQ[rawData, {}], Return[]];
  propList = {"Time", props};
  rule = Map[#[[1, 1]] ->
      Association[Thread[Rule[propList, Rest@Transpose[#]]]] &,
    GatherBy[rawData, First]];
  keys = Keys[Person];
  MapThread[
   If[MemberQ[keys, #1],
     AppendTo[Person[#1], Values[#2]],
     AppendTo[Person, #2]] &,
   {{id}, rule}];
  ];

getPerson[{id___Integer}, {props__}, Time_] :=
 Module[{query, rawData, data, rule, propList, keys},
  query =
   "SELECT PersonID, Time, " <> (Sequence @@ Riffle[{props}, ", "]) <>
     " FROM PersonValues where (PersonID=" <>
    Sequence @@ Riffle[ToString /@ {id}, " or PersonID="] <>
    ") and Time < " <> ToString[Time];
  rawData = SQLExecute[conn, query];
  If[MatchQ[rawData, {}], Return[]];
  propList = {"Time", props};
  rule = Map[#[[1, 1]] ->
      Association[Thread[Rule[propList, Rest@Transpose[#]]]] &,
    GatherBy[rawData, First]];
  keys = Keys[Person];
  MapThread[
   If[MemberQ[keys, #1],
     AppendTo[Person[#1], Values[#2]],
     AppendTo[Person, #2]] &,
   {{id}, rule}];
  ];

getPerson[{id___Integer}, {props__}, Time1_, Time2_] :=
 Module[{query, rawData, data, rule, propList, keys},
  query =
   "SELECT PersonID, Time, " <> (Sequence @@ Riffle[{props}, ", "]) <>
     " FROM PersonValues where (PersonID=" <>
    Sequence @@ Riffle[ToString /@ {id}, " or PersonID="] <>
    ") and (Time > " <> ToString[Time1] <> " and Time < " <>
    ToString[Time2] <> ")";
  Print[query];
  rawData = SQLExecute[conn, query];
  If[MatchQ[rawData, {}], Return[]];
  propList = {"Time", props};
  rule = Map[#[[1, 1]] ->
      Association[Thread[Rule[propList, Rest@Transpose[#]]]] &,
    GatherBy[rawData, First]];
  keys = Keys[Person];
  MapThread[
   If[MemberQ[keys, #1],
     AppendTo[Person[#1], Values[#2]],
     AppendTo[Person, #2]] &,
   {{id}, rule}];
  ]

extractPerson[id_, {props__}] := Module[{},
  Map[Thread[{Person[id]["Time"], Person[id][#]}] &,{props}]]

getConnections[conn_] :=
 Module[{conquery, perquery, conData, personIDs, perData},
  conquery = "SELECT PersonA, PersonB, Weight FROM Connections";
  conData = SQLExecute[conn, conquery]
  ]

getGraph[{id___}] :=
 Module[{conquery, perquery, conData, personIDs, perData},
  conquery = "SELECT PersonA, PersonB, Weight FROM Connections where (PersonA=" \
    <> Sequence @@ Riffle[ToString /@ {id}, " or PersonA="] <> ")" <>
      " or (PersonB=" <>Sequence @@ Riffle[ToString /@ {id}, " or PersonB="] <>
      ")";
  conData = SQLExecute[conn, conquery];
  connections = conData[[All, 1 ;; 2]] /. {{a_, b_} :> a <-> b};
  weights = conData[[All, 3]];
  Graph[connections]
  ]


(*============================================================================*)

(*============================Population Data=================================*)
computeProbability[data_] :=
 Module[{d, AllSizes, AllCounts, TotalProb, Probs},
  d = SortBy[Tally[data], First];
  AllSizes = d[[All, 1]];
  AllCounts = d[[All, 2]];
  TotalProb = Total[d /. {{s_Integer, n_Integer} :> s*n}];
  Probs =
   Table[Total[d[[i ;; -1]] /. {{s_Integer, n_Integer} :> s*n}]/
     TotalProb, {i, Length[AllCounts]}];
  Thread[{Sort@AllSizes, Probs}]
  ];

<<<<<<< HEAD
ar = 0.5;
=======
ar = 7/24;
>>>>>>> current

Options[getLogData] = {AxisRange->Automatic}

getLogData[db_String, OptionsPattern[]] :=
 Module[{res, conn, data, d, pd, model, line, r2},
  res = Association[{}];
  conn = OpenSQLConnection[
    JDBC["MySQL(Connector/J)", "127.0.0.1:3306/" <> db],
    "Username" -> "root", "Password" -> ""];
  data = SQLSelect[conn, "HistoryData", {"Time", "Infected"}][[7300 ;; -1]];
  d = Most@Map[Total, DeleteCases[Split[data[[All, 2]], # != 0 &], {0}]];
  pd = computeProbability[d];
  model = LinearModelFit[Log@pd[[4 ;; Floor[0.8 Length[pd]]]], x, x];
  line = model["BestFit"];
  r2 = model["RSquared"];
  {sr, fr} = model[{"StandardizedResiduals", "FitResiduals"}];

  AppendTo[res, "Epidemic Data" -> ListPlot[d,
     PlotRange -> {Automatic,{0,OptionValue[AxisRange]}},
     PlotStyle -> {{Thick, SIRRed}},
     PlotRange -> Full,
     Frame -> True,
<<<<<<< HEAD
     FrameLabel -> {Style["Epidemic Number", FontSize -> 26],
       Style["Epidemic Size", FontSize -> 26]},
=======
     FrameLabel -> {Style["Epidemic Number", FontSize -> 38],
       Style["Epidemic Size", FontSize -> 38]},
     FrameTicksStyle -> Directive["Label", 25],
>>>>>>> current
     LabelStyle -> Directive[Medium, Bold],
     ImageSize -> Large, PlotStyle -> 96, Filling -> Axis,
     AspectRatio -> ar]];
  AppendTo[res, "Histogram" ->
    Histogram[d, {200}, ImageSize -> Large,
     PlotRange->{{0,OptionValue[AxisRange]}, Automatic},
     ChartElementFunction -> "FadingRectangle", ChartStyle -> Orange,
     Frame -> True,
<<<<<<< HEAD
     FrameLabel -> {Style["Epidemic Size", FontSize -> 26],
       Style["Count", FontSize -> 26]}, AspectRatio -> ar,
=======
     FrameLabel -> {Style["Epidemic Size", FontSize -> 38],
       Style["Count", FontSize -> 38]},
     FrameTicksStyle -> Directive["Label", 25],
     AspectRatio -> ar,
>>>>>>> current
     LabelStyle -> Directive[Medium, Bold]]];
  AppendTo[res, "Log Probability Distribution" ->
    ListLogLogPlot[pd, Joined -> True, Mesh -> Full, ImageSize -> Large,
      PlotRange -> {{20,OptionValue[AxisRange]}, {Min[0.1, Min[pd]], 1.1}},
      GridLines -> Automatic, Frame -> True,
<<<<<<< HEAD
      FrameLabel -> {Style["Epidemic Size", FontSize -> 26],
      Style["Probability", FontSize -> 26]}, AspectRatio -> ar,
=======
      FrameLabel -> {Style["Epidemic Size", FontSize -> 38],
      Style["Probability", FontSize -> 38]},
      FrameTicksStyle -> Directive["Label", 25],
      AspectRatio -> ar,
>>>>>>> current
      LabelStyle -> Directive[Medium, Bold]]];
  AppendTo[res, "Log Distribution" -> ListLogLogPlot[Reverse@Sort@d,
     Joined -> True,
     Mesh -> Full,
     ImageSize -> Large,
     GridLines -> Automatic,
     Frame -> True,
     FrameLabel -> {Style["Epidemic Size", Large],
       Style["Number of Occurence", Large]}, AspectRatio -> ar,
     LabelStyle -> Directive[Medium, Bold]]];
  AppendTo[res, "Statistics Model" -> model];
  AppendTo[res, "Linear Model" ->
    Plot[line, {x, 0, pd[[-1, 1]]}, PlotRange -> Full,
     PlotStyle -> {Thick, Orange}, AspectRatio -> ar]];
  AppendTo[res, "R^2 Value" -> r2];
  AppendTo[res, "Error Statistics" -> {sr, fr}];
  AppendTo[res, "Model Parameters" -> model["ParameterTable"]];
  res
  ]


(*=============================Plotting=======================================*)



PopulationDataPlot[data_, opts___] := ListLinePlot[data,
  PlotStyle -> {{Thick, SIRBlue}, {Thick, SIRRed}, {Thick,
    SIRGreen}, {Thick, SIRBlack}},
  (*PlotLegends\[Rule]Placed[{"Susceptible","Infected","Recovered",
  "Dead"},Above],*)
  Frame -> True,
  FrameLabel -> {Style["Time", Large], Style["Population", Large]},
  LabelStyle -> Directive[Medium, Bold],
  ImageSize -> Large,
  opts]

CellDataPlot[data_, opts___] := ListLinePlot[data,
  PlotStyle -> {{Thick, SIRBlue, Dashed}, {Thick, SIRRed,
     Dotted}, {Thick, SIRGreen}, {Thick, SIRBlack}},
  PlotLegends ->
   Placed[{"Susceptible Cells", "Infected Cells", "Viral Load"},
    Right],
  Frame -> True,
  FrameLabel -> {Style["Time (days)", Medium], Style["Cells", Medium]},
  LabelStyle -> Directive[Medium, Bold],
  ImageSize -> Large,
  opts]
(*============================================================================*)

(*=============================Contact Network================================*)

End[]


EndPackage[]
