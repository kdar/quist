using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Net;
using System.Diagnostics;
using System.Windows.Forms;

namespace QuistLoader
{
  public class Config
  {
		private string _AvailableVersion;
		public string AvailableVersion 
		{ get { return _AvailableVersion; } set { _AvailableVersion = value; } }

		private string _AppFileURL;
		public string AppFileURL 
		{ get { return _AppFileURL; } set { _AppFileURL = value; } }

		private string _LatestChanges;
		public string LatestChanges 
		{ get { return _LatestChanges; } set { _LatestChanges = value; } }

		private string _ChangeLogURL;
		public string ChangeLogURL 
		{ get { return _ChangeLogURL; } set { _ChangeLogURL = value; } }

		public delegate void LoadConfigError(string stMessage, Exception e);
		public event LoadConfigError OnLoadConfigError;

    public bool LoadConfig(string url, string user, string pass, string proxyURL, bool proxyEnabled)
		{
			try {
				XmlDocument XmlDoc = new XmlDocument();
				HttpWebResponse Response;
				HttpWebRequest Request;

				Request = (HttpWebRequest)HttpWebRequest.Create(url);

				if(user != null && user != "")
					Request.Credentials = new NetworkCredential(user, pass);
				else
					Request.Credentials = CredentialCache.DefaultCredentials;

				if(proxyEnabled == true)
					Request.Proxy = new WebProxy(proxyURL,true);

				Response = (HttpWebResponse)Request.GetResponse();

				Stream respStream = null;
				respStream = Response.GetResponseStream();

				//Load the XML from the stream
				XmlDoc.Load(respStream);

				//Parse out the AvailableVersion
				XmlNode AvailableVersionNode = XmlDoc.SelectSingleNode(@"//AvailableVersion");
				this.AvailableVersion = AvailableVersionNode.InnerText;

				//Parse out the AppFileURL
				XmlNode AppFileURLNode = XmlDoc.SelectSingleNode(@"//AppFileURL");
				this.AppFileURL = AppFileURLNode.InnerText;

				//Parse out the LatestChanges
				XmlNode LatestChangesNode = XmlDoc.SelectSingleNode(@"//LatestChanges");
				if(LatestChangesNode != null)
					this.LatestChanges = LatestChangesNode.InnerText;
				else
					this.LatestChanges = "";

				//Parse out the ChangLogURL
				XmlNode ChangeLogURLNode = XmlDoc.SelectSingleNode(@"//ChangeLogURL");
				if(ChangeLogURLNode != null)
					this.ChangeLogURL = ChangeLogURLNode.InnerText;
				else
					this.ChangeLogURL = "";			
			} catch (Exception e) {
				string stMessage = "Failed to read the config file at: " + url + "\r\nMake sure that the config file is present and has a valid format.";
        Debug.WriteLine(stMessage);
				if(this.OnLoadConfigError != null)
					this.OnLoadConfigError(stMessage, e);

				return false;
			}

			return true;
		}
  }
}
