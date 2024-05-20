use std::fmt::{Display, Formatter};
use std::fs::{File, read_dir, create_dir_all, remove_file};
use std::io::{Read, Write, BufRead, Lines, BufReader};
use std::path::Path;
pub use super::variable_types::{VariableType as VarType, VariableType};
use log::{info, warn, error, debug};

struct VarStorageEntry {
    name: String,
    data: VarType,
    desc: String,
    write_to_files: bool
}
impl PartialEq for VarStorageEntry {
    fn eq(&self, other: &Self) -> bool {
        self.data == other.data
    }
}
impl Display for VarStorageEntry {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "${}:{}", self.name, self.data.display_type())
    }
}
impl VarStorageEntry {
    fn new(n: &str, data: VarType, write_to_files: bool) -> Option<Self> {
        if n.is_empty() {
            None
        }
        else {
            Some(Self {
                name: n.to_string(),
                data,
                desc: String::new(),
                write_to_files
            })
        }

    }

    fn set_writing_to_files(&mut self, new: bool) {
        self.write_to_files = new;
    }

    fn save_to_path(&self, path: &str) -> bool {
        if !self.write_to_files {
            error!("Attempting to write to file '{path}', but 'write_to_files' is false!");
            return false;
        }

        let file_t = File::create(path);
        match file_t {
            Ok(mut file) => {
                let sterilized = self.data.sterilize();
                file.write(sterilized.as_bytes()).is_ok()
            }
            Err(e) => {
                error!("Save to Path: Could not save to path because of '{}'!", e.to_string());
                false
            }
        }
    }
    fn load_from_path(&mut self, path: &str) -> bool {
        info!("Load Data from File: Loading variable at '{path}'");

        let file_t = File::open(path);
        let result: VariableType;
        match file_t {
            Ok(mut file) => {
                let mut contents: String = String::new();
                if let Err(e) = file.read_to_string(&mut contents) { 
                    error!("Load Data from File: Could not read from file because '{}'", e.to_string());
                    return false; 
                }

                let sterilized_val = VariableType::from_sterilize(&contents);
                if let Ok(val) = sterilized_val {
                    debug!("Load Data from File: Value loaded.");
                    result = val;
                }
                else { 
                    debug!("Load Data from File: Could not serilize value.");
                    return false; 
                }
            }
            Err(e) => {
                error!("Load Data from File: Could not open file because '{}'", e.to_string());
                return false;
            }
        }

        self.data = result;
        true
    }
    
    fn get_desc(&self) -> &str {
        &self.desc
    }

    fn get_data(&self) -> &VariableType {
        &self.data
    }
    fn get_data_mut(&mut self) -> &mut VariableType {
        &mut self.data
    }
    //Returns false if path is_some and saving to a path failed.
    fn set_data(&mut self, new_data: VariableType, path: Option<&str>) -> bool {
        self.data = new_data;

        path.is_none() || self.save_to_path(path.unwrap())
    }
}

pub struct VarStorage {
    variables: Vec<VarStorageEntry>,
    temporaries: Vec<VarStorageEntry>,
    env_vars: Vec<VarStorageEntry>,
    ans: VarStorageEntry,
    root: String, 
    save_to_files: bool
}
impl VarStorage {
    pub fn new(root_directory: &str) -> Option<Self> {
        debug!("VarStorage: Seeing if directory '{root_directory}' exists...");
        if !Path::new(root_directory).exists() {
            debug!("VarStorage: No, does not exist. Attempting to create...");
            if create_dir_all(root_directory).is_err(){
                error!("VarStorage: Directory could not be created!");
                return None;
            }  
        }

        info!("VarStorage: Sucessfully created.");
        Some(Self {
            variables: Vec::<VarStorageEntry>::new(),
            temporaries: Vec::<VarStorageEntry>::new(),
            env_vars: Vec::<VarStorageEntry>::new(),
            ans: VarStorageEntry::new("ans", VariableType::None, true).unwrap(),
            root: root_directory.to_string(),
            save_to_files: true
        })
    }

    pub fn is_saving_to_files(&self) -> bool {
        self.save_to_files
    }
    pub fn set_save_to_files(&mut self, new: bool) {
        info!("VarStorage: Set save_to_files mode to {new}");
        if self.save_to_files != new {
            self.save_to_files = new;

            for v in &mut self.variables {
                v.set_writing_to_files(new);
            }
            self.ans.set_writing_to_files(new);
        }
    }

    fn get_variable_path(&self, name: &str) -> String {
        if name == "ans" {
            format!("{}ans", self.root)
        }
        else {
            format!("{}v.{}", self.root, name)
        }
    }

    pub fn index_variables(&mut self) -> bool {
        //This function will see what variables there are in the root directory
        info!("VarStorage Index: Attempting index.");

        if !Path::new(&self.root).exists() {
            info!("VarStorage Index: Could not index because directory does not exist.");
            self.save_to_files = false;
            return false;
        }

        debug!("VarStorage Index: Clearing previous data.");

        self.temporaries.clear();
        self.variables.clear();
        self.ans.set_data(VariableType::None, None);

        debug!("VarStorage Index: Previous data cleared.");

        if let Ok(contents) = read_dir(&self.root) {
            for item in contents.flatten() {
                let file_name = item.file_name().to_str().unwrap().trim().to_string();
                let path = item.path().to_str().unwrap().trim().to_string();
        
                debug!("VarStorage Index: Indexing variable '{file_name}' at file '{path}'");

                if file_name.len() < 3 {
                    warn!("VarStorage Index: File name is not of proper format, as it must be 3 characters. Skipping.");
                    continue;
                }

                if file_name == "ans" {
                    //Load ans
                    debug!("VarStorage Index: Loading ans...");

                    if !self.ans.load_from_path(&path) {
                        warn!("VarStorage Index: Could not index 'Ans', skipping.");
                    }
                }
                else if let Some(stripped) = file_name.strip_prefix("v.") {
                    //Load variable

                    if self.variables.iter().any(|x| x.name == stripped) {
                        error!("Duplicate variable found!!");
                        continue;
                    }

                    let new_var: &mut VarStorageEntry;
                    {
                        let new = VarStorageEntry::new(stripped, VariableType::None, self.save_to_files);
                        if let Some(n) = new {
                            self.variables.push(n);
                            new_var = self.variables.last_mut().unwrap();
                        }
                        else {
                            error!("Could not parse variable, skipping.");
                            continue;
                        }
                    }

                    if !new_var.load_from_path(&path) {
                        error!("Could not load variable '{stripped}'!");
                        continue;
                    }
                }
                else {
                    warn!("VarStorage Index: Could not index '{file_name}', as it is not in format. Skipping.");
                    continue;
                }
            }

            info!("VarStorage Index: Sucessfully indexed variables.");
            self.set_save_to_files(true);
            true
        }
        else {
            self.set_save_to_files(false);
            false
        }
    }
    pub fn variable_exists(&self, name: &str) -> bool {
        self.get_variable(name).is_some()
    }

    fn get_variable_index(&self, name: &str) -> Option<usize> {
        for (i, item) in self.variables.iter().enumerate(){
            if item.name == name {
                return Some(i);
            }
        }
        
        None
    }
    fn get_variable(&self, name: &str) -> Option<&VarStorageEntry> {
        self.variables.iter().find(|x| x.name == name)
    }
    fn get_variable_mut(&mut self, name: &str) -> Option<&mut VarStorageEntry> {
        if name.is_empty() {
            return None;
        }

        info!("Get Var: Getting variable with name '${name}'");

        {
            debug!("Get Var: Checking to see if it already exists...");
            let index = self.get_variable_index(name);
            if let Some(i) = index {
                info!("Get Var: Variable found.");
                return Some(&mut self.variables[i]);
            }
        }

        debug!("Get Var: Does not exist, attempting to create...");
        {
            let index = self.add_variable(name, VariableType::None);
            if let Some(i) = index {
                Some(&mut self.variables[i])
            }
            else {
                None
            }
        }
    }

    pub fn load_variable(&mut self, name: &str) -> bool {
        let path = self.get_variable_path(name);
        let var = self.get_variable_mut(name);

        var.is_some() && var.unwrap().load_from_path(&path)
    }
    pub fn reset_variable(&mut self, name: &str) -> bool {
        let target_r = self.get_variable_mut(name);
        if target_r.is_none() {
            return false;
        }

        let target = target_r.unwrap();
        target.data = VariableType::None;

        true
    }
    //Removes from the current instance.
    pub fn pop_variable(&mut self, name: &str) -> bool {
        let index = self.get_variable_index(name);

        if let Some(i) = index {
            self.variables.remove(i);

            true
        }
        else {
            false
        }
    }
    //Removes from file system and drops
    pub fn drop_variable(&mut self, name: &str) -> bool {
        self.pop_variable(name) && remove_file(self.get_variable_path(name)).is_ok()
    }

    pub fn get_variable_value(&self, name: &str) -> Option<&VariableType> {
        Some(&self.get_variable(name)?.data)
    }
    pub fn get_variable_value_mut(&mut self, name: &str) -> Option<&mut VariableType> {
        Some(&mut self.get_variable_mut(name)?.data)
    }
    pub fn set_variable_value(&mut self, name: &str, new_value: VariableType) -> bool {
        let path = format!("{}/v.{}", &self.root, name);

        let target_r = self.get_variable_mut(name);
        if let Some(target) = target_r {
            target.data = new_value;

            target.save_to_path(&path)
        }
        else {
            false
        }
    }
    pub fn add_variable(&mut self, name: &str, value: VariableType) -> Option<usize> {
        if let Some(i) = self.get_variable_index(name){
            return Some(i);
        }

        if !self.save_to_files {
            let new_entry = VarStorageEntry::new(name, value, self.save_to_files);
            if new_entry.is_none() {
                error!("The new variable could not be created.");
                return None;
            }

            self.variables.push(new_entry.unwrap());

            let index = self.variables.len() - 1;
            info!("Get Var: Variable found.");
            return Some(index);
        }

        {
            let path = self.get_variable_path(name);
            debug!("Get Var: Attempting to write to file '{path}'...");

            let file_t = File::create(path);
            if let Ok(mut file) = file_t {
                debug!("Get Var: File created, attempting to write...");
                if file.write_all(value.sterilize().as_bytes()).is_err() {
                    error!("Get Var: Could not write to file.");
                    return None;
                }

                debug!("Get Var: File written, variable returned.");
                let new_entry = VarStorageEntry::new(name, value, self.save_to_files);
                self.variables.push(new_entry.unwrap());

                let index = self.variables.len() - 1;
                info!("Get Var: Variable found.");
                Some(index)
            }
            else {
                info!("Get Var: Variable could not be found or created.");
                None
            } 
        }
    }

    pub fn get_temporary_value(&self, name: &str) -> Option<&VariableType> {
        Some(&self.temporaries.iter().find(|x| x.name == name)?.data)
    }   
    pub fn get_temporary_value_mut(&mut self, name: &str) -> Option<&mut VariableType> {
        Some(&mut self.temporaries.iter_mut().find(|x| x.name == name)?.data)
    }
    pub fn set_temporary_value(&mut self, name: &str, new_data: VariableType) -> bool {
        let target = self.temporaries.iter_mut().find(|x| x.name == name);

        if let Some(t) = target {
            t.data = new_data;
            true
        }
        else {
            false
        }
    }
    pub fn drop_temporary(&mut self, name: &str) -> bool {
        for (i, item) in self.variables.iter().enumerate(){
            if item.name == name {
                self.temporaries.remove(i);
                return true;
            }
        }

        false
    }

    fn get_env_var(&self, name: &str) -> Option<&VarStorageEntry> {
        self.env_vars.iter().find(|x| x.name == name)
    }
    fn get_env_var_mut(&mut self, name: &str) -> Option<&mut VarStorageEntry> {
        self.env_vars.iter_mut().find(|x| x.name == name)
    }
    pub fn get_env_var_value(&self, name: &str) -> Option<&VariableType> {
        Some(&self.get_env_var(name)?.data)
    }
    pub fn get_env_var_value_mut(&mut self, name: &str) -> Option<&VariableType> {
        Some(&mut self.get_env_var_mut(name)?.data)
    }
    pub fn get_env_var_desc(&self, name: &str) -> Option<&str> {
        Some(&self.get_env_var(name)?.get_desc())
    }
    pub fn set_env_var_value(&self, name: &str, val: VariableType) -> bool {
        todo!();
    }
    pub fn set_env_var_desc(&self, name: &str, desc: Option<String>) -> bool {
        todo!();
    }

    pub fn get_ans(&self) -> &VariableType {
        self.ans.get_data()
    }
    pub fn get_ans_mut(&mut self) -> &mut VariableType {
        self.ans.get_data_mut()
    }
    pub fn set_ans(&mut self, new_ans: VariableType) -> bool {
        if self.save_to_files {
            let path = self.get_variable_path("ans");
            self.ans.set_data(new_ans, Some(&path))
        }
        else {
            self.ans.set_data(new_ans, None)
        }
    }
}

fn read_lines<P>(filename: P) -> std::io::Result<Lines<BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(std::io::BufReader::new(file).lines())
}