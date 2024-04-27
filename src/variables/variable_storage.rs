use std::fmt::{Display, Formatter};
use std::fs::{File, read_dir, create_dir_all};
use std::io::{Read, Write};
use std::path::Path;
pub use super::variable_types::{VariableType as VarType, VariableType};
use log::{info, warn, error, debug};

struct VarStorageEntry {
    name: String,
    data: VarType
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
    fn new(n: &str, data: VarType) -> Option<Self> {
        if n.is_empty() {
            None
        }
        else {
            Some(Self {
                name: n.to_string(),
                data
            })
        }

    }
}

pub struct VarStorage {
    variables: Vec<VarStorageEntry>,
    temporaries: Vec<VarStorageEntry>,
    ans: VariableType,
    root: String, 
    save_to_files: bool
}
impl VarStorage {
    pub fn new(root_directory: &str) -> Option<Self> {
        debug!("VarStorage: Seeing if directory {root_directory} exists...");
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
            ans: VariableType::None,
            root: root_directory.to_string(),
            save_to_files: true
        })
    }

    pub fn is_saving_to_files(&self) -> bool {
        self.save_to_files
    }
    pub fn save_to_files(&mut self, new: bool) {
        info!("VarStorage: Set save_to_files mode to {new}");
        self.save_to_files = new;
    }

    pub fn index_variables(&mut self) -> bool {
        //This function will see what variables there are in the root directory
        info!("VarStorage Index: Attempting index.");
        debug!("VarStorage Index: Clearing previous data.");

        self.temporaries.clear();
        self.variables.clear();
        self.ans = VariableType::None;

        debug!("VarStorage Index: Previous data cleared.");

        if !Path::new(&self.root).exists() {
            info!("VarStorage Index: Could not index because directory does not exist.");
            self.save_to_files = false;
            return false;
        }

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

                    let result = self.load_variable_data_from_file(&path);
                    if let Some(a) = result {
                        self.ans = a;
                    }
                    else {
                        warn!("VarStorage Index: Ans could not be loaded, skipping ans.");
                        self.ans = VariableType::None;
                        continue;
                    }
                }
                else if let Some(stripped) = file_name.strip_prefix("v.") {
                    //Load variable
                    if !self.load_variable_from_file(&path, stripped) {
                        warn!("VarStorage Index: Could not index '{file_name}', skipping.");
                        continue;
                    }
                }
                else {
                    warn!("VarStorage Index: Could not index '{file_name}', as it is not in format. Skipping.");
                    continue;
                }
            }

            info!("VarStorage Index: Sucessfully indexed variables.");
            self.save_to_files = true;
            true
        }
        else {
            self.save_to_files = false;
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

        if !self.save_to_files {
            debug!("Get Var: Save to file is off, so adding to array...");

            let new_entry = VarStorageEntry::new(name, VariableType::None);
            self.variables.push(new_entry.unwrap());

            let index = self.variables.len() - 1;
            info!("Get Var: Variable found.");
            return Some(&mut self.variables[index]);
        }

        debug!("Get Var: Does not exist, attempting to create...");
        //It does not exist in the list, so we will make one
        {
            let path = format!("{}v.{}", &self.root, name);
            debug!("Get Var: Attempting to write to file '{path}'...");
            let file_t = File::create(path);
            if let Ok(mut file) = file_t {
                debug!("Get Var: File created, attempting to write...");
                if file.write_all(VariableType::None.sterilize().as_bytes()).is_err() {
                    error!("Get Var: Could not write to file.");
                    return None;
                }

                debug!("Get Var: File written, variable returned.");
                let new_entry = VarStorageEntry::new(name, VariableType::None);
                self.variables.push(new_entry.unwrap());

                let index = self.variables.len() - 1;
                info!("Get Var: Variable found.");
                Some(&mut self.variables[index])
            }
            else {
                info!("Get Var: Variable could not be found or created.");
                None
            } 
        }
    }

    fn load_variable_data_from_file(&mut self, path: &str) -> Option<VariableType> {
        info!("Load Data from File: Loading variable at '{path}'");

        let file_t = File::open(path);
        let result: VariableType;
        match file_t {
            Ok(mut file) => {
                let mut contents: String = String::new();
                if let Err(e) = file.read_to_string(&mut contents) { 
                    error!("Load Data from File: Could not read from file because '{}'", e.to_string());
                    return None; 
                }

                let sterilized_val = VariableType::from_sterilize(&contents);
                if let Ok(val) = sterilized_val {
                    debug!("Load Data from File: Value loaded.");
                    result = val;
                }
                else { 
                    debug!("Load Data from File: Could not serilize value.");
                    return None; 
                }
            }
            Err(e) => {
                error!("Load Data from File: Could not open file because '{}'", e.to_string());
                return None;
            }
        }

        Some(result)
    }
    fn load_variable_from_file(&mut self, path: &str, name: &str) -> bool {
        info!("Load from File: Attempting to load variable '{name}' from file '{path}'");

        debug!("Load from File: Extracting from file...");
        let extracted = self.load_variable_data_from_file(path);
        if extracted.is_none() { 
            error!("Load from File: Extraction failed!");
            return false; 
        }

        debug!("Load from File: Attempting to load variable in memory...");
        match self.get_variable_mut(name) {
            Some(target) => {
                debug!("Load from File: Target aquired, filling with data.");
                target.data = extracted.unwrap();

                info!("Load from File: Variable '{name}' has been loaded with '{}'", target.data.display_type());
                true
            }
            None => {
                error!("Load from File: Unable to load from memory!");
                false
            }
        }
    }
    pub fn load_variable(&mut self, name: &str) -> bool {
        let path = format!("{}/v.{}", &self.root, &name);

        self.load_variable_from_file(&path, name)
    }
    pub fn release_variable(&mut self, name: &str) -> bool {
        let target_r = self.get_variable_mut(name);
        if target_r.is_none() {
            return false;
        }

        let target = target_r.unwrap();
        target.data = VariableType::None;

        true
    }
    pub fn get_variable_value(&self, name: &str) -> Option<&VariableType> {
        let target = self.get_variable(name);
        match target {
            Some(v) => Some(&v.data),
            None => None
        }
    }
    pub fn get_variable_value_mut(&mut self, name: &str) -> Option<&mut VariableType> {
        let target = self.get_variable_mut(name);
        match target {
            Some(v) => Some(&mut v.data),
            None => None
        }
    }
    pub fn set_variable_value(&mut self, name: &str, new_value: VariableType) -> bool {
        let path = format!("{}/v.{}", &self.root, name);

        let target_r = self.get_variable_mut(name);
        if target_r.is_none() {
            return false;
        }

        let target = target_r.unwrap();

        let file_t = File::create(path);
        match file_t {
            Ok(mut file) => {
                let sterilized = new_value.sterilize();
                if file.write(sterilized.as_bytes()).is_err() { return false; }
            }
            _ => return false
        }

        target.data = new_value;
        true
    }

    pub fn get_temporary_value(&self, name: &str) -> Option<&VariableType> {
        let target = self.temporaries.iter().find(|x| x.name == name);

        if let Some(t) = target {
            Some(&t.data)
        }
        else {
            None
        }
    }   
    pub fn get_temporary_value_mut(&mut self, name: &str) -> Option<&mut VariableType> {
        let target = self.temporaries.iter_mut().find(|x| x.name == name);

        if let Some(t) = target {
            Some(&mut t.data)
        }
        else {
            None
        }
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

    pub fn get_ans(&self) -> &VariableType {
        &self.ans
    }
    pub fn set_ans(&mut self, new_ans: VariableType) -> bool {
        self.ans = new_ans;
        let path = format!("{}ans", &self.root);

        if let Ok(mut file)=  File::create(path) {
            let sterilized = self.ans.sterilize();
            if file.write(sterilized.as_bytes()).is_ok() { 
                return true; 
            }
        }  

        false
    }
}
