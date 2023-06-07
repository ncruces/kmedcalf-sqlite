drop table if exists IANAZones;
create virtual table if not exists temp.IANAZones using statement((
with params(x_lat, x_lon, radius) as
     (
        values (radians(coalesce(:latitude, 0)), radians(coalesce(:longitude, 0)), 6371008.8)
     ),
     centroids(d_lat, x_lon, y_lon, Radius, Zone) as
     (
        select abs(radians(lat) - x_lat),
               x_lon,
               radians(lon),
               radius,
               Zone
          from params, tz_centroids, tz_zones
         where tz_centroids.Zone_ID == tz_Zones.ID
     ),
     candidates(Distance, Zone) as
     (
        select abs(radius * atan2(sqrt(pow(cos(y_lon) * sin(d_lat), 2) + pow(cos(x_lon) * sin(y_lon) - sin(x_lon) * cos(y_lon) * cos(d_lat), 2)),
                                  (sin(x_lon)*sin(y_lon) + cos(x_lon)*cos(y_lon)*cos(d_lat))
                                 )
                  ),
               Zone
          from centroids
     )
  select Candidates.Distance,
         TZ_Zones.Zone,
         TZ_Countries.Country,
         TZ_Zones.Area,
         TZ_Zones.Location,
         TZ_Zones.City,
         TZ_Centroids.lat,
         TZ_Centroids.lon
    from candidates
    join TZ_Zones
      on Candidates.Zone == TZ_Zones.Zone
    join TZ_Centroids
      on TZ_Centroids.Zone_ID == TZ_Zones.ID
    join TZ_Countries
      on TZ_Countries.Code == TZ_Zones.Country_Code
order by Candidates.Distance
   limit 5
));

