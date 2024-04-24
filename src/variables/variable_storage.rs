use std::fmt::{Display, Formatter};
use std::fs::{File, read_dir};
use std::io::{Read, Write};
use std::path::Path;
use super::variable_types::{VariableType as VarType, VariableType};

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
    root: String
}
impl VarStorage {
    pub fn new(root_directory: &str) -> Option<Self> {
        if !Path::new(root_directory).exists() {
            None
        }
        else {
            Some(Self {
                variables: Vec::<VarStorageEntry>::new(),
                temporaries: Vec::<VarStorageEntry>::new(),
                ans: VariableType::None,
                root: root_directory.to_string()
            })
        }
    }

    pub fn index_variables(&mut self) -> bool {
        //This function will see what variables there are in the root directory

        if let Ok(contents) = read_dir(&self.root) {
            for item in contents.flatten() {
                let file_name = item.file_name().to_str().unwrap().to_string();
                let path = item.path().to_str().unwrap().to_string();
                if file_name.len() < 2 {
                    return false;
                }

                if file_name == "ans" {
                    //Load ans
                    let result = self.load_variable_data_from_file(&path);
                    if let Some(a) = result {
                        self.ans = a;
                    }
                    else {
                        return false;
                    }
                }
                else if file_name == "$" {
                    //Load variable
                    if !self.load_variable_from_file(&path, &file_name[1..]) {
                        return false;
                    }
                }
                else { return false; }
            }

            true
        }
        else {
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

        {
            let index = self.get_variable_index(name);
            if let Some(i) = index {
                return Some(&mut self.variables[i]);
            }
        }

        //It does not exist in the list, so we will make one
        {
            let path = format!("{}${}", &self.root, name);
            let file_t = File::create(path);
            if let Ok(mut file) = file_t {
                if file.write_all(VariableType::None.sterilize().as_bytes()).is_err() {
                    return None;
                }

                let new_entry = VarStorageEntry::new(name, VariableType::None);
                self.variables.push(new_entry.unwrap());

                let index = self.variables.len() - 1;
                Some(&mut self.variables[index])
            }
            else {
                None
            } 
        }
    }

    fn load_variable_data_from_file(&mut self, path: &str) -> Option<VariableType> {
        if !Path::new(path).exists() {
            return None;
        }

        let file_t = File::open(path);
        let result: VariableType;
        match file_t {
            Ok(mut file) => {
                let mut contents: String = String::new();
                if file.read_to_string(&mut contents).is_err() { return None; }

                let sterilized_val = VariableType::from_sterilize(&contents);
                if let Ok(val) = sterilized_val {
                    result = val;
                }
                else { return None; }
            }
            _ => return None
        }

        Some(result)
    }
    fn load_variable_from_file(&mut self, path: &str, name: &str) -> bool {
        if !Path::new(path).exists() {
            return false;
        }

        let extracted = self.load_variable_data_from_file(path);
        if extracted.is_none() { return false; }

        match self.get_variable_mut(name) {
            Some(target) => target.data = extracted.unwrap(),
            None => {
                let entry = VarStorageEntry::new(name, extracted.unwrap());
                if let Some(a) = entry {
                    self.variables.push(a);
                }
                else {
                    return false;
                }
            }
        }

        true
    }
    pub fn load_variable(&mut self, name: &str) -> bool {
        let path = format!("{}/${}", &self.root, &name);
        if !Path::new(&path).exists() {
            return false;
        }

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
        let mut path = format!("{}/$", &self.root);
        if !Path::new(&path).exists() {
            return false;
        }

        let target_r = self.get_variable_mut(name);
        if target_r.is_none() {
            return false;
        }

        let target = target_r.unwrap();
        path += &target.name;

        let file_t = File::create(&path);
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
