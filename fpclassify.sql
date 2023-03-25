with x(x) as
     (
        values (1e999),
               (1.7976931348623157e+308),
               (1e50),
               (2.2250738585072014e-308),
               (2.2250738585072014e-308/16),
               (0.0),
               (-0.0),
               (-2.2250738585072014e-308/16),
               (-2.2250738585072014e-308),
               (-1e50),
               (-1.7976931348623157e+308),
               (-1e999)
     )
select x,
       isDenormal(x),
       isInfinity(x),
       isNormal(x),
       isPInfinity(x),
       isPNormal(x),
       isPDenormal(x),
       isPZero(x),
       isNZero(x),
       isNDenormal(x),
       isNNormal(x),
       isNInfinity(x)
  from x;

