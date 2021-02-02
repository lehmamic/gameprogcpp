using System;
using System.Collections.Generic;
using System.IO;
using System.Numerics;
using System.Text.Json;

namespace Chapter06
{
    public class Mesh : IDisposable
    {
        private readonly List<ProcessedTexture> _textures = new();

        // Get the vertex array associated with this mesh
        public VertexArray VertexArray { get; private set; }

        public string ShaderName { get; private set; }

        // Get object space bounding sphere radius
        public float Radius { get; private set; }
        
        // Get specular power of mesh
        public float SpecularPower { get; private set; } = 100.0f;
        
        // Get a texture from specified index
        public ProcessedTexture GetTexture(int index)
        {
            return _textures[index];
        }

        // Load/unload mesh
        public bool Load(string fileName, Renderer renderer)
        {

            try
            {
                string json = File.ReadAllText(fileName);

                var options = new JsonDocumentOptions
                {
                    AllowTrailingCommas = true
                };

                using JsonDocument document = JsonDocument.Parse(json, options);

                // Check the version
                int version = document.RootElement.GetProperty("version").GetInt32();
                if (version != 1)
                {
                    Console.WriteLine($"Mesh {version} not version 1");
                    return false;
                }
                
                ShaderName = document.RootElement.GetProperty("shader").GetString();

                // Skip the vertex format/shader for now
                // (This is changed in a later chapter's code)
                const int vertexSize = 8;

                // Load textures
                var texturesJson = document.RootElement.GetProperty("textures");
                if (texturesJson.ValueKind != JsonValueKind.Array || texturesJson.GetArrayLength() < 1)
                {
                    Console.WriteLine($"Mesh {fileName} has no textures, there should be at least one");
                    return false;
                }

                SpecularPower = (float)document.RootElement.GetProperty("specularPower").GetDouble();

                foreach (JsonElement texture in texturesJson.EnumerateArray())
                {
                    // Is this texture already loaded?
                    var textureName = texture.GetString();
                    ProcessedTexture t = renderer.GetTexture(textureName);
                    if (t is null)
                    {
                        // Try loading the texture
                        t = renderer.GetTexture(textureName);
                        if (t is null)
                        {
                            // If it's still null, just use the default texture
                            t = renderer.GetTexture("Assets/Default.png");
                        }
                    }
        
                    _textures.Add(t);
                }
                
                // Load in the vertices
                var verticesJson = document.RootElement.GetProperty("vertices");
                if (verticesJson.ValueKind != JsonValueKind.Array || verticesJson.GetArrayLength() < 1)
                {
                    Console.WriteLine($"Mesh {fileName} has no vertices");
                    return false;
                }

                var vertices = new List<Vertex>();
                Radius = 0.0f;
    
                foreach (JsonElement vertexJson in verticesJson.EnumerateArray())
                {
                    // For now, just assume we have 8 elements
                    if (vertexJson.ValueKind != JsonValueKind.Array || vertexJson.GetArrayLength() != vertexSize)
                    {
                        Console.WriteLine($"Unexpected vertex format for {fileName}");
                        return false;
                    }

                    Vector3 position = new Vector3((float)vertexJson[0].GetDouble(), (float)vertexJson[1].GetDouble(), (float)vertexJson[2].GetDouble());
                    Radius = Math.Max(Radius, position.LengthSquared());

                    var vertex = new Vertex(
                        position,
                        new Vector3((float) vertexJson[3].GetDouble(), (float) vertexJson[4].GetDouble(),
                            (float) vertexJson[5].GetDouble()),
                        new Vector2((float) vertexJson[6].GetDouble(), (float) vertexJson[7].GetDouble()));

                    vertices.Add(vertex);
                }

                // We were computing length squared earlier
                Radius = (float)Math.Sqrt(Radius);

                // Load in the indices
                var indicesJson = document.RootElement.GetProperty("indices");
                if (indicesJson.ValueKind != JsonValueKind.Array || indicesJson.GetArrayLength() < 1)
                {
                    Console.WriteLine($"Mesh {fileName} has no indices");
                    return false;
                }

                var indices = new List<ushort>();
                foreach (JsonElement index in indicesJson.EnumerateArray())
                {
                    if (index.ValueKind != JsonValueKind.Array || index.GetArrayLength() != 3)
                    {
                        Console.WriteLine($"Invalid indices for {fileName}");
                        return false;
                    }

                    indices.Add(index[0].GetUInt16());
                    indices.Add(index[1].GetUInt16());
                    indices.Add(index[2].GetUInt16());
                }

                // Now create a vertex array
                VertexArray = new VertexArray(renderer.GraphicsDevice, vertices.ToArray(), indices.ToArray());
    
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }
        }

        public void Dispose()
        {
            VertexArray.Dispose();
            VertexArray = null;
            _textures.Clear();
        }
    }
}