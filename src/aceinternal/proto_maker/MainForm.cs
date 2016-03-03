using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Xml;
using System.Diagnostics;

namespace proto_maker
{
    public partial class MainForm : Form
    {
        private Hashtable template_hash_table;
        private ProtoFile[] proto_files;
        private string head_file_path;
        private string cpp_file_path;
        private string template_file_path;
        private string proto_xml_file_path;
        private string proto_path;
        private string proto_db_out;
        private string[] proto_file_list;
        private string db_script_file_path;
        private StreamWriter head_sw;
        private StreamWriter cpp_sw;
        private StreamWriter script_sw;
        public MainForm()
        {
            InitializeComponent();
            LoadConfigFile();
            LoadCppTemplateFile();
        }

        private bool LoadConfigFile()
        {
            XmlDocument doc = new XmlDocument();
            doc.Load("proto_maker_config.xml");
            XmlNode root = doc.DocumentElement;
            head_file_path = root.SelectSingleNode("head-file").Attributes["path"].Value;
            cpp_file_path = root.SelectSingleNode("cpp-file").Attributes["path"].Value;
            template_file_path = root.SelectSingleNode("template-file").Attributes["path"].Value;
            proto_xml_file_path = root.SelectSingleNode("proto-xml-file").Attributes["path"].Value;
            proto_path = root.SelectSingleNode("proto").Attributes["path"].Value;
            proto_db_out = root.SelectSingleNode("proto").Attributes["db-out"].Value;
            proto_file_list = new string[root.SelectSingleNode("proto").ChildNodes.Count];
            int index = 0;
            foreach (XmlNode file in root.SelectSingleNode("proto").ChildNodes)
            {
                proto_file_list[index] = proto_path + file.Attributes["path"].Value;
                index++;
            }
            db_script_file_path = root.SelectSingleNode("db-script-file").Attributes["path"].Value;
            /// 
            head_sw = new StreamWriter(head_file_path);
            cpp_sw = new StreamWriter(cpp_file_path);
            script_sw = new StreamWriter(db_script_file_path);
            return true;
        }

        private bool LoadCppTemplateFile()
        {
            template_hash_table = new Hashtable();
            string line = "";
            string block_name = "";
            try
            {
                using (StreamReader sr = new StreamReader(template_file_path))
                {
                    while ((line = sr.ReadLine()) != null)
                    {
                        if (line.Length >= 4 && line.Substring(0, 4) == "/*$-")
                        {
                            ArrayList block = new ArrayList();
                            block_name = line.Substring(4, line.Length - 4);
                            block_name = block_name.Trim();
                            while ((line = sr.ReadLine()) != null && line != "-$*/")
                                block.Add(line);
                            template_hash_table[block_name] = block;
                        }
                    }
                }
                return true;
            }
            catch (Exception e)
            {
                // Let the user know what went wrong.
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
                return false; 
            }
        }

        private bool LoadProtoXmlFile()
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(proto_xml_file_path);
            XmlNode root = doc.DocumentElement;
            proto_files = new ProtoFile[root.ChildNodes.Count];
            int index_file = 0;
            foreach (XmlNode file in root.ChildNodes)
            {
                proto_files[index_file] = new ProtoFile();
                proto_files[index_file].name = file.Name;
                proto_files[index_file].ns = file.Attributes["NS"].Value;
                proto_files[index_file].messages = new ProtoMessage[file.ChildNodes.Count];
                int index_message = 0;
                foreach (XmlNode message in file.ChildNodes) 
                {
                    proto_files[index_file].messages[index_message] = new ProtoMessage();
                    proto_files[index_file].messages[index_message].name = message.Name;
                    if ((message.Name.Length > 4) && (message.Name.Substring(0, 4) == "Tmp_"))
                        proto_files[index_file].messages[index_message].is_temp = true;
                    else
                        proto_files[index_file].messages[index_message].is_temp = false;
                    proto_files[index_file].messages[index_message].members = new ProtoMessageMember[message.ChildNodes.Count];
                    int index_member = 0;
                    bool has_guid = false;
                    foreach (XmlNode member in message.ChildNodes) 
                    {
                        proto_files[index_file].messages[index_message].members[index_member] = new ProtoMessageMember();
                        proto_files[index_file].messages[index_message].members[index_member].name = member.Name;
                        proto_files[index_file].messages[index_message].members[index_member].cpp_type = member.Attributes["CPP-TYPE"].Value;
                        proto_files[index_file].messages[index_message].members[index_member].db_type = member.Attributes["DB-TYPE"].Value;
                        if (member.Attributes["STORE"].Value == "required")
                            proto_files[index_file].messages[index_message].members[index_member].store = 1;
                        else if (member.Attributes["STORE"].Value == "optional")
                            proto_files[index_file].messages[index_message].members[index_member].store = 2;
                        else if (member.Attributes["STORE"].Value == "repeated")
                            proto_files[index_file].messages[index_message].members[index_member].store = 3;
                        if (member.Name == "guid" && 
                          member.Attributes["STORE"].Value == "required" && 
                          member.Attributes["CPP-TYPE"].Value == "::boost::uint64_t")
                            has_guid = true;
                        index_member++;
                    }
                    if (!has_guid)
                        return false;
                    index_message++;
                }
                index_file++;
            }
            return true;
        }

        private void print(ref StreamWriter sw)
        {
            sw.WriteLine();
        }

        private void print(ArrayList al, ref StreamWriter sw)
        {
            foreach (string str in al)
            {
                sw.WriteLine(str);
            }
        }

        private void print(ArrayList al, Hashtable ht, ref StreamWriter sw)
        {
            foreach(string str in al)
            {
                string str1 = str;
                foreach (DictionaryEntry de in ht)
                {
                    str1 = str1.Replace(de.Key.ToString(), de.Value.ToString());
                }
                sw.WriteLine(str1);
            }
        }

        private bool GeneriteHeadFile()
        {
            /// Write warning 
            print((ArrayList) template_hash_table["H-WARNING"], ref head_sw);
            print(ref head_sw);
            /// Write ifdef top
            print((ArrayList) template_hash_table["H-IFDEF-TOP"], ref head_sw);
            print(ref head_sw);
            /// Write static inc
            print((ArrayList) template_hash_table["H-STATIC-INC"], ref head_sw);
            print(ref head_sw);
            /// Write entity inc
            foreach (ProtoFile pf in proto_files)
            {
                Hashtable ht = new Hashtable();
                ht["$OBJECT$"] = pf.name;
                print((ArrayList) template_hash_table["H-ENTITY-INC"], ht, ref head_sw);
            }
            print(ref head_sw);
            /// Write namespace top
            print((ArrayList)template_hash_table["NAMESPACE-TOP"], ref head_sw);
            print(ref head_sw);
            /// Write CacheAssistant interface 
            print((ArrayList)template_hash_table["H-CACHEASSISTANT-DEFINE"], ref head_sw);
            print(ref head_sw);
            /// Write Ass class define
            foreach (ProtoFile pf in proto_files)
            {
                foreach(ProtoMessage pm in pf.messages)
                {
                    Hashtable ht = new Hashtable();
                    ht["$OBJECT$"] = pm.name;
                    ht["$OBJECT_NAMESPACE$"] = pf.ns;
                    if (pm.is_temp)
                        ht["$IS-TEMP$"] = "true";
                    else
                        ht["$IS-TEMP$"] = "false";
                    print((ArrayList)template_hash_table["H-ASS-CLASS-DEFINE"], ht, ref head_sw);
                    print(ref head_sw);
                } 
            }
            /// Write namespace buttom
            print((ArrayList) template_hash_table["NAMESPACE-BUTTOM"], ref head_sw);
            print(ref head_sw);
            /// Write ifdef buttom
            print((ArrayList) template_hash_table["H-IFDEF-BUTTOM"], ref head_sw);
            /// flush and close file
            head_sw.Flush();
            head_sw.Close();
            return true;
        }

        private bool GeneriteCppFile()
        {
            /// Write warning
            print((ArrayList) template_hash_table["CPP-WARNING"], ref cpp_sw);
            print(ref cpp_sw);
            /// Write inc
            print((ArrayList) template_hash_table["CPP-INC"], ref cpp_sw);
            print(ref cpp_sw);
            /// Write namespace top
            print((ArrayList) template_hash_table["NAMESPACE-TOP"], ref cpp_sw);
            print(ref cpp_sw);
            /// Generite function
            foreach (ProtoFile pf in proto_files)
            {
                foreach (ProtoMessage pm in pf.messages)
                {
                    if (pm.is_temp)
                    {
                        Hashtable ht = new Hashtable();
                        ht["$OBJECT$"] = pm.name;
                        ht["$FUNC$"] = "load";
                        print((ArrayList) template_hash_table["CPP-TMP-FUNC-IMPL"], ht, ref cpp_sw);
                        print(ref cpp_sw);
                        ht["$FUNC$"] = "insert";
                        print((ArrayList) template_hash_table["CPP-TMP-FUNC-IMPL"], ht, ref cpp_sw);
                        print(ref cpp_sw);
                        ht["$FUNC$"] = "update";
                        print((ArrayList) template_hash_table["CPP-TMP-FUNC-IMPL"], ht, ref cpp_sw);
                        print(ref cpp_sw);
                        ht["$FUNC$"] = "remove";
                        print((ArrayList) template_hash_table["CPP-TMP-FUNC-IMPL"], ht, ref cpp_sw);
                        print(ref cpp_sw);
                    }
                    else
                    {
                        GeneriteInsert(pm);
                        print(ref cpp_sw);
                        GeneriteLoad(pm);
                        print(ref cpp_sw);
                        GeneriteUpdate(pm);
                        print(ref cpp_sw);
                        GeneriteRemove(pm);
                        print(ref cpp_sw);
                    }  
                }
            }
            /// Write namespace buttom
            print((ArrayList) template_hash_table["NAMESPACE-BUTTOM"], ref cpp_sw);
            /// 
            cpp_sw.Flush();
            cpp_sw.Close();
            return true;
        }

        private bool GeneriteInsert(ProtoMessage pm)
        {
            Hashtable ht = new Hashtable();
            ht["$OBJECT$"] = pm.name;
            /// Write top
            print((ArrayList) template_hash_table["CPP-INSERT-DEFINE-TOP"], ht, ref cpp_sw);
            /// Write insert code
            bool first_time = true;
            foreach (ProtoMessageMember pmm in pm.members)
            {
                ht["$MEMBER$"] = pmm.name;
                ht["$CPP-TYPE$"] = pmm.cpp_type;
                if (first_time)
                {
                    first_time = false;
                }
                else
                {
                    print((ArrayList) template_hash_table["CPP-INSERT-QUERY-SPEC"], ref cpp_sw);
                }
                if (pmm.store == 3)
                {
                    if (pmm.cpp_type == "::std::string")
                        print((ArrayList) template_hash_table["CPP-INSERT-REPEATED-STRING-MEMBER-QUERY"], ht, ref cpp_sw);
                    else if (isMessageType(pmm.cpp_type))
                        print((ArrayList)template_hash_table["CPP-INSERT-REPEATED-MESSAGE-MEMBER-QUERY"], ht, ref cpp_sw);
                    else
                        print((ArrayList) template_hash_table["CPP-INSERT-REPEATED-NONSTRING-MEMBER-QUERY"], ht, ref cpp_sw);
                }
                else if (pmm.cpp_type == "::std::string")
                    print((ArrayList) template_hash_table["CPP-INSERT-STRING-MEMBER-QUERY"], ht, ref cpp_sw);
                else
                    print((ArrayList) template_hash_table["CPP-INSERT-NONSTRING-MEMBER-QUERY"], ht, ref cpp_sw);
            }
            /// Write buttom
            print((ArrayList) template_hash_table["CPP-INSERT-DEFINE-BUTTOM"], ht, ref cpp_sw);
            return true;
        }

        private bool isMessageType(string cpp_type)
        {
            return cpp_type.Contains("entity");
        }

        private bool GeneriteLoad(ProtoMessage pm)
        {
            Hashtable ht = new Hashtable();
            ht["$OBJECT$"] = pm.name;
            ht["$MEMBER-LIST$"] = "";
            foreach (ProtoMessageMember pmm in pm.members)
            {
                ht["$MEMBER-LIST$"] += pmm.name;
                ht["$MEMBER-LIST$"] += ",";
            }
            string temp = ht["$MEMBER-LIST$"].ToString();
            if (temp.Substring(temp.Length - 1, 1) == ",")
                ht["$MEMBER-LIST$"] = temp.Remove(temp.Length - 1, 1);
            /// Write top
            print((ArrayList) template_hash_table["CPP-LOAD-DEFINE-TOP"], ht, ref cpp_sw);
            /// Write member load code
            int index = 0;
            foreach (ProtoMessageMember pmm in pm.members)
            {
                ht["$MEMBER$"] = pmm.name;
                ht["$CPP-TYPE$"] = pmm.cpp_type;
                ht["$INDEX$"] = Convert.ToString(index);   
                if (pmm.store == 3)
                {
                    if (pmm.cpp_type == "::std::string")
                        print((ArrayList)template_hash_table["CPP-LOAD-REPEATED-STRING-MEMBER-FILL"], ht, ref cpp_sw);
                    else if (isMessageType(pmm.cpp_type))
                        print((ArrayList)template_hash_table["CPP-LOAD-REPEATED-MESSAGE-MEMBER-FILL"], ht, ref cpp_sw);
                    else
                        print((ArrayList)template_hash_table["CPP-LOAD-REPEATED-NONSTRING-MEMBER-FILL"], ht, ref cpp_sw);
                }
                else if (pmm.cpp_type == "::std::string")
                    print((ArrayList) template_hash_table["CPP-LOAD-STRING-MEMBER-FILL"], ht, ref cpp_sw);
                else
                    print((ArrayList) template_hash_table["CPP-LOAD-NONSTRING-MEMBER-FILL"], ht, ref cpp_sw);
                index++;
            }
            /// Write buttom
            print((ArrayList) template_hash_table["CPP-LOAD-DEFINE-BUTTOM"], ht, ref cpp_sw);

            return true;
        }

        private bool GeneriteUpdate(ProtoMessage pm)
        {
            Hashtable ht = new Hashtable();
            ht["$OBJECT$"] = pm.name;
            /// Write top
            print((ArrayList) template_hash_table["CPP-UPDATE-DEFINE-TOP"], ht, ref cpp_sw);
            /// Write insert code
            bool first_time = true;
            foreach (ProtoMessageMember pmm in pm.members)
            {
                ht["$MEMBER$"] = pmm.name;
                ht["$CPP-TYPE$"] = pmm.cpp_type;
                if (first_time)
                {
                    first_time = false;
                }
                else
                {
                    print((ArrayList) template_hash_table["CPP-INSERT-QUERY-SPEC"], ref cpp_sw);
                }
                if (pmm.store == 3)
                {
                    if (pmm.cpp_type == "::std::string")
                        print((ArrayList) template_hash_table["CPP-INSERT-REPEATED-STRING-MEMBER-QUERY"], ht, ref cpp_sw);
                    else if (isMessageType(pmm.cpp_type))
                        print((ArrayList)template_hash_table["CPP-INSERT-REPEATED-MESSAGE-MEMBER-QUERY"], ht, ref cpp_sw);
                    else
                        print((ArrayList) template_hash_table["CPP-INSERT-REPEATED-NONSTRING-MEMBER-QUERY"], ht, ref cpp_sw);
                }
                else if (pmm.cpp_type == "::std::string")
                    print((ArrayList) template_hash_table["CPP-INSERT-STRING-MEMBER-QUERY"], ht, ref cpp_sw);
                else
                    print((ArrayList) template_hash_table["CPP-INSERT-NONSTRING-MEMBER-QUERY"], ht, ref cpp_sw);
            }
            /// Write buttom
            print((ArrayList) template_hash_table["CPP-UPDATE-DEFINE-BUTTOM"], ht, ref cpp_sw);
            return true;
        }

        private bool GeneriteRemove(ProtoMessage pm)
        {
            Hashtable ht = new Hashtable();
            ht["$OBJECT$"] = pm.name;
            /// Write top
            print((ArrayList) template_hash_table["CPP-REMOVE-DEFINE"], ht, ref cpp_sw);
            return true;
        }

        private bool GeneriteDBScript()
        {
            foreach (ProtoFile pf in proto_files)
            {
                foreach (ProtoMessage pm in pf.messages)
                {
                    if (!pm.is_temp)
                    {
                        Hashtable ht = new Hashtable();
                        ht["$OBJECT$"] = pm.name;
                        print((ArrayList) template_hash_table["DBSCRIPT-TABLE-TOP"], ht, ref script_sw);
                        foreach (ProtoMessageMember pmm in pm.members)
                        {
                            ht["$OBJECT$"] = pmm.name;
                            ht["$DB-TYPE$"] = pmm.db_type;
                            print((ArrayList)template_hash_table["DBSCRIPT-TABLE-FIELD-DEF"], ht, ref script_sw);
                        }
                        ht["$OBJECT$"] = pm.name;
                        print((ArrayList) template_hash_table["DBSCRIPT-TABLE-BUTTOM"], ht, ref script_sw);
                        print(ref script_sw);
                    }
                }
            }
            /// 
            print((ArrayList)template_hash_table["DBSCRIPT-PING-TABLE"], ref script_sw);
            /// 
            script_sw.Flush();
            script_sw.Close();
            return true;
        }

        private void RunProto_Click(object sender, EventArgs e)
        {
            System.Diagnostics.ProcessStartInfo psi = new System.Diagnostics.ProcessStartInfo();
            psi.FileName = "cache_message_tool.exe";
            psi.Arguments = "--proto_path=" + proto_path + " --db_out=" + proto_db_out;
            psi.Arguments += " " + proto_path;

//             foreach(string str in proto_file_list)
//             {
//                psi.Arguments += " ";
//                psi.Arguments += str;
//             }

            psi.RedirectStandardOutput = true; 
            psi.WindowStyle = System.Diagnostics.ProcessWindowStyle.Normal; 
            psi.UseShellExecute = false; 
            System.Diagnostics.Process listFiles; 
            listFiles = System.Diagnostics.Process.Start(psi); 
            System.IO.StreamReader myOutput = listFiles.StandardOutput; 
            //listFiles.WaitForExit();
            this.output.Text = myOutput.ReadToEnd();
            //this.output.Text = "finish";
            if (listFiles.ExitCode == 0)
            {
                LoadProtoXmlFile();
                GeneriteHeadFile();
                GeneriteCppFile();
                GeneriteDBScript();
            }
        }
    }

    public class Configure
    {
        public string template_file_path;
        public string db_proto_exe_path;
        public string db_proto_xml_file_path;
        public string db_proto_file_path;
        public string[] db_proto_files;
    }

    public class ProtoMessageMember
    {
        public string name;
        public int store;
        public string cpp_type;
        public string db_type;
    }

    public class ProtoMessage
    {
        public string name;
        public bool is_temp;
        public ProtoMessageMember[] members;
    }

    public class ProtoFile
    {
        public string name;
        public string ns;
        public ProtoMessage[] messages;
    }
}

/*$- DBSCRIPT-TABLE-FIELD-DEF
    `$OBJECT$` $DB-TYPE$ $SIGN$ $DEFAULT-VALUE$,
-$*/

/*$- DBSCRIPT-TABLE-TOP
DROP TABLE IF EXISTS `$OBJECT$`;
CREATE TABLE `$OBJECT$` (
-$*/

/*$- DBSCRIPT-TABLE-BUTTOM
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-$*/