namespace Launcher
{
    using System;

    /// <summary>
    /// An immutable class for storing information about a connected controller. Provides methods for converting from/to strings.
    /// </summary>
    public class GamePad
    {
        private Guid guid;
        private string deviceName;
        private string instanceName;

        /// <summary>
        /// Default constructor that for defining all properties of the controller.
        /// </summary>
        /// <param name="guid">The device GUID.</param>
        /// <param name="deviceName">The device category name.</param>
        /// <param name="instanceName">The device instance name.</param>
        public GamePad(Guid guid, string deviceName, string instanceName)
        {
            this.guid = guid;
            this.deviceName = deviceName;
            this.instanceName = instanceName;
        }

        /// <summary>
        /// Converts a string of the format "deviceName,instanceName,GUID" to <see cref="GamePad"/>  object.
        /// </summary>
        /// <param name="input">A string of the format "deviceName,instanceName,GUID".</param>
        /// <returns>Return a <see cref="GamePad"/> object converted from the input string.</returns>
        public static GamePad Parse(string input)
        {
            string[] args = input.Split(',');
            if (args.Length != 3)
            {
                throw new FormatException();
            }

            Guid g = Guid.Parse(args[2]);
            return new GamePad(g, args[0], args[1]);
        }

        /// <inheritdoc />
        public override string ToString()
        {
            return $"{deviceName}:{instanceName}";
        }

        /// <inheritdoc />
        public override int GetHashCode()
        {
            return guid.GetHashCode();
        }

        /// <inheritdoc />
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }

            GamePad pad = obj as GamePad;
            if (pad == null)
            {
                return false;
            }

            return pad.guid == guid;
        }

        /// <summary>
        /// Gets the <see cref="Guid"/> of this controller.
        /// </summary>
        public Guid Guid
        {
            get
            {
                return guid;
            }
        }
    }
}
