﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMS_WebDesignCore.IBusiness.IDusiness_AI_Interpreter
{
    public interface ITranslationService
    {
        Task<List<string>> TranslateText(List<string> texts, string targetLanguage);
    }
}