void datapoint_test(){

   NucDBDataPoint * point0 = new NucDBDataPoint(0.5,0.2,0.1);
   NucDBDataPoint * point1 = new NucDBDataPoint(2.5,0.5,0.3);

   NucDBDataPoint * point = new NucDBDataPoint( *point0 );
   (*point) += (*point1);
   point->Print();

}
