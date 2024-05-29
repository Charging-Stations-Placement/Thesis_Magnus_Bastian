using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using System.Formats.Asn1;
using System.Globalization;
using CsvHelper;
using CsvHelper.Configuration;
using System.Runtime.InteropServices;
using DotSpatial.Data;
using NetTopologySuite.Geometries;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using System.Text.Json;
using NetTopologySuite.Index.Strtree;
using System.Collections.Concurrent;
using Newtonsoft.Json;

namespace MapApp.Pages
{
    /*public class ShapefileLoader
    {
        public static List<List<List<(double, double)>>> LoadShapefile(string filePath)
        {
            var allPolygons = new List<List<List<(double, double)>>>();

            // Load the shapefile
            IFeatureSet featureSet = FeatureSet.Open(filePath);

            // Iterate through each feature in the shapefile
            foreach (var feature in featureSet.Features)
            {
                var polygons = new List<List<(double, double)>>();

                if (feature.Geometry is Polygon polygon)
                {
                    polygons.Add(ConvertCoordinates(polygon.Shell.Coordinates));
                    foreach (var hole in polygon.Holes)
                    {
                        polygons.Add(ConvertCoordinates(hole.Coordinates));
                    }
                }
                else if (feature.Geometry is MultiPolygon multiPolygon)
                {
                    foreach (Polygon poly in multiPolygon.Geometries)
                    {
                        var polyPoints = ConvertCoordinates(poly.Shell.Coordinates);
                        polygons.Add(polyPoints);
                        foreach (var hole in poly.Holes)
                        {
                            polygons.Add(ConvertCoordinates(hole.Coordinates));
                        }
                    }
                }

                allPolygons.Add(polygons);
            }
            // Serialize and save to JSON
            var json = JsonConvert.SerializeObject(allPolygons);
            File.WriteAllText("Polygons.json", json);
            return allPolygons;
        }

        // New method to flatten polygons and convert to an array format
        public static double[] FlattenPolygons(List<List<List<(double, double)>>> polygons, out int totalPointsCount)
        {
            var flatList = new List<double>();
            totalPointsCount = 0;

            foreach (var polygon in polygons)
            {
                foreach (var ring in polygon)
                {
                    foreach (var point in ring)
                    {
                        flatList.Add(point.Item2); // Latitude
                        flatList.Add(point.Item1); // Longitude
                        totalPointsCount++;
                    }
                }
            }

            return flatList.ToArray();
        }
        private static List<(double, double)> ConvertCoordinates(Coordinate[] coordinates)
        {
            var pointsList = new List<(double, double)>();
            foreach (var coordinate in coordinates)
            {
                pointsList.Add((coordinate.X, coordinate.Y)); // Longitude, Latitude
            }
            return pointsList;
        }

        public static void LoadShapefileAndSaveToCustomFormat(string filePath, string outputPath)
        {
            var allPolygons = new List<List<List<(double, double)>>>();

            // Load the shapefile
            IFeatureSet featureSet = FeatureSet.Open(filePath);

            // Iterate through each feature in the shapefile
            foreach (var feature in featureSet.Features)
            {
                var polygons = new List<List<(double, double)>>();

                if (feature.Geometry is Polygon polygon)
                {
                    polygons.Add(ConvertCoordinates(polygon.Shell.Coordinates));
                    foreach (var hole in polygon.Holes)
                    {
                        polygons.Add(ConvertCoordinates(hole.Coordinates));
                    }
                }
                else if (feature.Geometry is MultiPolygon multiPolygon)
                {
                    foreach (Polygon poly in multiPolygon.Geometries)
                    {
                        var polyPoints = ConvertCoordinates(poly.Shell.Coordinates);
                        polygons.Add(polyPoints);
                        foreach (var hole in poly.Holes)
                        {
                            polygons.Add(ConvertCoordinates(hole.Coordinates));
                        }
                    }
                }

                allPolygons.Add(polygons);
            }

            // Serialize to a custom format and save
            using (var writer = new StreamWriter(outputPath))
            {
                foreach (var polygonList in allPolygons)
                {
                    writer.WriteLine($"POLYGON_COUNT: {polygonList.Count}");
                    foreach (var polygon in polygonList)
                    {
                        writer.WriteLine($"POINT_COUNT: {polygon.Count}");
                        foreach (var point in polygon)
                        {
                            writer.WriteLine($"{point.Item2},{point.Item1}");
                        }
                    }
                }
            }
        }
    }*/

    public class GetPoints
    {
        public static List<GravityData> LoadPointsFromFile(string filePath)
        {
            List<GravityData> points = new List<GravityData>();
            string[] lines = File.ReadAllLines(filePath);

            foreach (var line in lines.Skip(1)) // Skip the header line
            {
                string[] parts = line.Split(',');

                if (parts.Length >= 3 && double.TryParse(parts[0], NumberStyles.Any, CultureInfo.InvariantCulture, out double longitude) &&
                    double.TryParse(parts[1], NumberStyles.Any, CultureInfo.InvariantCulture, out double latitude) &&
                    double.TryParse(parts[2], NumberStyles.Any, CultureInfo.InvariantCulture, out double gravity))
                {
                    points.Add(new GravityData(longitude, latitude, gravity));
                }
            }

            return points;
        }
    }

    public static class NativeMethods
    {
        [DllImport("GravityScoreDLL.dll")]
        public static extern void calculateScore();

        [DllImport("GeosDLL.dll")]
        public static extern void GeneratePointsInGrid(double NELat, double NELng, double SWLat, double SWLng, int gridSize);
    }
    public class GravityData
    {
        public double longitude { get; set; }
        public double latitude { get; set; }
        public double gravity { get; set; }

        public GravityData(double longitude, double latitude, double gravity)
        {
            this.longitude = longitude;
            this.latitude = latitude;
            this.gravity = gravity;
        }
    }



    public class MapModel : PageModel
    {
        private readonly IWebHostEnvironment _environment;

        public List<GravityData> GravityPoints { get; private set; }

        public MapModel(IWebHostEnvironment environment)
        {
            _environment = environment;
        }

        [HttpGet]
        public IActionResult OnGetUpdateBounds(double northEastLat, double northEastLng, double southWestLat, double southWestLng)
        {
            NativeMethods.GeneratePointsInGrid(northEastLat, northEastLng, southWestLat, southWestLng, 50);

            NativeMethods.calculateScore();

            return new JsonResult(GetPoints.LoadPointsFromFile("gravityScoresWithData.csv"));

        }

        /*
        public void OnGet()
        {
            string currentDirectory = Environment.CurrentDirectory;
            Console.WriteLine("Current working directory: " + currentDirectory);

            string executableDirectory = AppDomain.CurrentDomain.BaseDirectory;
            Console.WriteLine("Executable directory: " + executableDirectory);

            double latitude = 54.628473, longitude = 11.436272;

            GravityPoints = new List<GravityData>();

            try
            {
                // Call a method from the DLL
                var result = NativeMethods.objectiveFunction(latitude, longitude);
                // If the call is successful, the DLL has been loaded
                Console.WriteLine("DLL loaded successfully. Function call result: " + result);
            }
            catch (DllNotFoundException dllNotFoundEx)
            {
                // This exception is thrown if the DLL could not be found
                Console.WriteLine("DLL not found: " + dllNotFoundEx.Message);
            }
            catch (EntryPointNotFoundException entryPointEx)
            {
                // This exception is thrown if the method (entry point) is not found in the DLL
                Console.WriteLine("Method not found in DLL: " + entryPointEx.Message);
            }
            catch (Exception ex)
            {
                // Handle other exceptions
                Console.WriteLine("Error calling method from DLL: " + ex.Message);
            }


            var gravityScore = NativeMethods.objectiveFunction(latitude, longitude);


            GravityPoints.Add(new GravityData { latitude = latitude, longitude = longitude, gravity = gravityScore });


            var filePath = Path.Combine(_environment.ContentRootPath, "gravityScoresWithData.csv");

            using (var reader = new StreamReader(filePath))
            using (var csv = new CsvReader(reader, new CsvConfiguration(CultureInfo.InvariantCulture) { HasHeaderRecord = true }))
            {
                GravityPoints = csv.GetRecords<GravityData>().ToList();
            }
        }*/
    }
}
