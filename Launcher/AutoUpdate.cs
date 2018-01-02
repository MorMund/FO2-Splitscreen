namespace Launcher
{
    using Newtonsoft.Json;
    using System;
    using System.Diagnostics;
    using System.IO;
    using System.Linq;
    using System.Net.Http;
    using System.Reflection;
    using System.Threading.Tasks;

    public static class AutoUpdate
    {
        private const string versionURI = "https://1smi6xsg92.execute-api.us-east-2.amazonaws.com/release";
        private const string verCacheFile = "FO2SS_CachedVersion.json";
        private const string flatOutDll = "FlatOut2.dll";
        private const int netRetryCount = 3;
        private static TimeSpan netRetryWait = new TimeSpan(0, 0, 0, 15, 0); // 15 seconds

        public static event VersionUpdateEvent NewVersion;
        public static event VersionUpdateEvent NotifyNewVersion;

        public static void CheckVersion()
        {
            try
            {
                Task.Run(() => AsyncCheckVersion());
            }
            catch (Exception e)
            {
                Console.WriteLine("Failed to check version {0}.", e);
            }
        }

        public static void SetNotifyUpdate(bool notify)
        {
            var ver = GetVersionCache();
            WriteVersionCache(notify, Version.Parse(ver.Version), ver.ValideUntil);
        }

        private static async void AsyncCheckVersion(int retry = 0)
        {
            var http = new HttpClient
            {
                BaseAddress = new Uri(versionURI)
            };

            var json = JsonSerializer.Create();
            var cachedVersion = GetCachedVersion(out var notify);
            var req = string.Empty;
            try
            {
                req = await http.GetStringAsync(string.Empty);
            }
            catch (HttpRequestException e)
            {
                Console.WriteLine("[Updater] Failed to get current version. Caused by");
                Console.WriteLine(e.ToString());
                if (retry < netRetryCount)
                {
                    AsyncCheckVersion(retry + 1);
                    return;
                }
            }

            using (JsonTextReader jsonReader = new JsonTextReader(new StringReader(req)))
            {
                var update = json.Deserialize<VersionUpdate>(jsonReader);
                var currentVer = Version.Parse(update.Version);
                var cacheValidTo = DateTime.UtcNow + update.CacheDuration;
                if (cachedVersion != null && cachedVersion >= currentVer)
                {
                    currentVer = cachedVersion;
                    Console.WriteLine("[Updater] Cached Version is newest Version.");
                }
                else
                {
                    Console.WriteLine($"[Updater] New version is available! Version {currentVer}");
                    notify = true;
                }

                if (currentVer > LauncherVersion)
                {
                    var args = new VersionUpdateEventArgs(update);
                    NewVersion?.Invoke(null, args);

                    if (notify)
                    {
                        NotifyNewVersion?.Invoke(null, args);
                    }
                }

                WriteVersionCache(notify, currentVer, cacheValidTo);
            }
        }

        private static void WriteVersionCache(bool notify, Version currentVer, DateTime cacheValidTo)
        {
            var json = JsonSerializer.Create();
            using (var updatedCache = new StringWriter())
            {
                json.Serialize(updatedCache, new VersionCache
                {
                    CheckedOn = DateTime.UtcNow,
                    ValideUntil = cacheValidTo,
                    Version = currentVer.ToString(),
                    Notify = notify
                });
                File.WriteAllText(CacheFilePath, updatedCache.ToString());
            }
        }

        private static VersionCache GetVersionCache()
        {
            var json = JsonSerializer.Create();
            using (var jsonReader = new JsonTextReader(File.OpenText(CacheFilePath)))
            {
                return json.Deserialize<VersionCache>(jsonReader);
            }
        }

        private static Version GetCachedVersion(out bool notify)
        {
            var json = JsonSerializer.Create();
            Version cachedVersion = null;
            notify = true;
            try
            {
                if (File.Exists(CacheFilePath))
                {
                    var verCache = GetVersionCache();
                    if (verCache.ValideUntil > DateTime.UtcNow)
                    {
                        Console.WriteLine("[Updater] Version cache is still valid.");
                        return null;
                    }
                    else
                    {
                        Version.TryParse(verCache.Version, out cachedVersion);
                        notify = verCache.Notify;
                    }

                }
            }
            catch (IOException e)
            {
                Console.WriteLine("[Updater] Failed to read version cache. Caused by");
                Console.WriteLine(e.ToString());
            }

            return cachedVersion;
        }

        public static Version FlatOutDLLVersion
        {
            get
            {
                FileVersionInfo dllVersion = null;
                try
                {
                    dllVersion = FileVersionInfo.GetVersionInfo(Path.Combine(Directory.GetCurrentDirectory(), flatOutDll));
                    // GetVersionInfo doesn't throw if the file is not found, but file name does
                    var a = dllVersion.FileName;
                }
                catch (ArgumentException)
                {
                    return new Version();
                }
                return Version.Parse(dllVersion.FileVersion ?? "0.0");
            }
        }

        public static Version LauncherVersion => Assembly.GetCallingAssembly().GetName().Version;

        private static string CacheFilePath => Path.Combine(Path.GetTempPath(), verCacheFile);

        [JsonObject]
        public struct VersionUpdate
        {
            [JsonProperty]
            public string Version { get; set; }
            [JsonProperty]
            public DateTime ReleaseDate { get; set; }
            [JsonProperty]
            public TimeSpan CacheDuration { get; set; }
            [JsonProperty]
            public string[] ChangeLog { get; set; }
        }

        public class VersionUpdateEventArgs : EventArgs
        {
            public VersionUpdate Version { get; private set; }
            public VersionUpdateEventArgs(VersionUpdate version)
            {
                Version = version;
            }
        }

        public delegate void VersionUpdateEvent(object sender, VersionUpdateEventArgs e);

        [JsonObject]
        private struct VersionCache
        {
            [JsonProperty]
            public string Version { get; set; }
            [JsonProperty]
            public DateTime ValideUntil { get; set; }
            [JsonProperty]
            public DateTime CheckedOn { get; set; }
            [JsonProperty]
            public bool Notify { get; set; }
        }
    }
}
